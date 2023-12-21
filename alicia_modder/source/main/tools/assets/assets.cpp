//
// Created by maros on 16/09/2021.
//

#include "assets.hpp"

#include <cwchar>
#include <fstream>
#include <string>
#include <zlib.h>

#include <filesystem>

/**
 * Calculate buffer's alicia checksum.
 * @param buffer Buffer
 * @param length Length
 * @return Checksum
 */
int32_t alicia_checksum(const char* buffer, uint64_t length)
{
  int32_t result = 0;
  do {
    result += *buffer;
    buffer++;
    length--;
  } while (length != 0);
  return result;
}

std::shared_ptr<uint8_t> read_embedded_data(std::ifstream& file, const assets::asset_common& header)
{
  if (!header.is_asset_embedded)
    return nullptr;

  // seek data
  const auto origin = file.tellg();
  file.seekg(header.embedded_data_offset);

  std::shared_ptr<uint8_t> data(nullptr);

  if (header.is_data_compressed)
  {
    // make room for decompressed data
    try
    {
      auto* ptr = new uint8_t[header.data_decompressed_length];
      data.reset(ptr);
    }
    catch (const std::bad_alloc& ex)
    {
      return nullptr;
    }

    // read them
    auto* buf = new uint8_t[header.embedded_data_length];
    file.read((char*) buf, header.embedded_data_length);

    // decompress them
    uncompress(
      data.get(), (uLongf*) &header.data_decompressed_length,
      buf, header.embedded_data_length);

    delete[] buf;
  }
  else
  {
    // make room for data nad read them
    data.reset(new uint8_t[header.embedded_data_length]);
    file.read((char*) data.get(), header.embedded_data_length);
  }

  if (file.eof())
    file.clear();
  // seek origin
  file.seekg(origin);

  return data;
}

bool write_embedded_data(std::ofstream& file, assets::asset_common& header, const std::shared_ptr<uint8_t>& data)
{
  if (!header.is_asset_embedded)
    return false;

  const auto origin = file.tellp();

  // seek next offset
  static uint32_t data_offset = assets::PAK_DATA_SECTOR_ADDR;
  file.seekp(data_offset);

  uLongf embedded_crc = 0;
  uLongf decompressed_crc = {
    crc32(0, data.get(), header.data_decompressed_length)};
  uint32_t decompressed_checksum = alicia_checksum((const char*) data.get(), header.data_decompressed_length);
  header.checksum_decompressed = decompressed_checksum;

  if (header.is_data_compressed)
  {
    uLongf compressedLength = header.embedded_data_length;
    // aprox. compressed length if not available
    if (!compressedLength)
      compressedLength = compressBound(header.data_decompressed_length);

    // compress with the highest level
    auto* compressedBuf = new uint8_t[compressedLength];
    compress2((Bytef*) compressedBuf, &compressedLength, data.get(), header.data_decompressed_length, 9);

    // calculate compressed crc
    embedded_crc = {
      crc32(0, compressedBuf, compressedLength)};
    // calculate compressed sum
    uint32_t embedded_checksum = alicia_checksum((const char*) compressedBuf, compressedLength);
    header.checksum_embedded = embedded_checksum;

    // embedded data length is now equal to compressed length
    header.embedded_data_length = compressedLength;

    file.write((char*) compressedBuf, compressedLength);
    delete[] compressedBuf;
  }
  else
  {
    // both crc's are the same
    embedded_crc = decompressed_crc;

    // in case header has been compressed before, update the embedded length.
    header.embedded_data_length = header.data_decompressed_length;

    file.write((char*) data.get(), header.embedded_data_length);
  }


  // update offset and crcs
  header.embedded_data_offset = data_offset;
  header.crc_decompressed = decompressed_crc;
  header.crc_embedded = embedded_crc;
  // skip 4 bytes after this data
  data_offset += header.embedded_data_length + 4;

  file.seekp(origin);
  return true;
}

std::shared_ptr<assets::PakAsset> read_pak_asset(std::ifstream& file, bool processData = true)
{
  std::shared_ptr<assets::PakAsset> asset(new assets::PakAsset{});
  file.read((char*) &asset->header.common, sizeof(asset->header.common));
  file.read((char*) &asset->header.w_path, sizeof(asset->header.w_path));
  for (int i = 0; i < 256; ++i)
  {
    wchar_t o = asset->header.w_path[i];
    if (o == 0)
      break;
    asset->header.path[i] = (char) o;
  }

  if (processData && asset->header.common.is_asset_embedded)
    asset->data = read_embedded_data(file, asset->header.common);

  return asset;
}


bool write_pak_asset(std::ofstream& file, const std::shared_ptr<assets::PakAsset>& asset, bool processData = true)
{
  if (processData && asset->header.common.is_asset_embedded)
    write_embedded_data(file, asset->header.common, asset->data);

  // write header
  file.write((char*) &asset->header.common, sizeof(asset->header.common));
  // write path
  file.write((char*) &asset->header.w_path, sizeof(asset->header.w_path));

  return true;
}


void assets::AssetsIO::read_pak_contents(const std::string& path, assets::PakContents& contents)
{
  contents.assets.clear();
  std::ifstream file(
    path, std::ios::binary);
  if (file.fail())
    throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), "File not found.");


  {
    file.read((char*) &contents.fileHeader,
              sizeof(pak_file_header));// read assets file header
  }
  if (file.fail())
    throw std::system_error(std::make_error_code(std::errc::invalid_argument), "File header couldn't be read.");

  {
    file.seekg(PAK_ASSET_HEADER_ADDR);
    file.read((char*) &contents.contentHeader,
              sizeof(pak_content_header));// read assets content header
  }
  if (file.fail())
  {
    throw std::system_error(std::make_error_code(std::errc::invalid_argument), "Content header couldn't be read.");
  }

  contents.assets.reserve(contents.contentHeader.asset_count);
  for (int assetIndex = 0; assetIndex < contents.contentHeader.asset_count; ++assetIndex)
  {
    auto asset = read_pak_asset(file, true);
    contents.assets.push_back(
      asset);
  }

  file.close();
}

void assets::AssetsIO::write_pak_contents(const std::string& path, assets::PakContents& contents)
{
  std::ofstream file(
    path, std::ios::binary);
  if (file.fail())
    throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), "Couldn't open file for reading.");

  {
    contents.contentHeader.asset_count = contents.assets.size();
    file.seekp(PAK_ASSET_HEADER_ADDR);
    file.write((char*) &contents.contentHeader,
               sizeof(pak_content_header));// write assets content header
  }
  if (file.fail())
    throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), "Couldn't write content header");

  size_t totalAssetCount = 0;
  size_t usedAssetCount = 0;
  size_t delAssetCount = 0;

  size_t compressedAssetCount = 0;
  size_t embeddedAssetCount = 0;

  for (int assetIndex = 0; assetIndex < contents.assets.size(); ++assetIndex)
  {
    const auto& asset = contents.assets[assetIndex];

    if (asset->header.common.is_data_compressed)
      compressedAssetCount++;
    usedAssetCount++;
    totalAssetCount++;

    if (asset->header.common.is_asset_deleted)
      delAssetCount++;
    if (asset->header.common.is_asset_embedded)
      embeddedAssetCount++;
    write_pak_asset(file, asset);
  }

  file.write((char*) &contents.dataHeader,
             sizeof(pak_data_header));// write assets data header
  if (file.fail())
    throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), "Couldn't write data header");

  file.seekp(0, std::ios_base::end);
  const auto file_size = file.tellp();

  // update file header
  contents.fileHeader.file_size = file_size;
  contents.fileHeader.asset_count = totalAssetCount;
  contents.fileHeader.used_assets_count = usedAssetCount;
  contents.fileHeader.deleted_assets_count = delAssetCount;

  {
    file.seekp(std::ios_base::beg);
    file.write((char*) &contents.fileHeader,
               sizeof(pak_file_header));// write assets file header
  }
  if (file.fail())
    throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory), "Couldn't write file header");

  file.close();
}

bool assets::AssetsIO::export_pak_asset(const std::string& parent_dir, const std::shared_ptr<PakAsset>& asset)
{
  const auto path = std::filesystem::path(parent_dir + "/" + asset->header.path);
  const auto parentPath = path.parent_path();

  std::filesystem::create_directories(parentPath);
  std::ofstream file(path, std::ios_base::binary);

  if (asset->header.common.is_data_compressed)
    file.write((char*) asset->data.get(), asset->header.common.data_decompressed_length);
  else
    file.write((char*) asset->data.get(), asset->header.common.embedded_data_length);

  file.close();
  return false;
}