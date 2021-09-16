//
// Created by maros on 16/09/2021.
//

#include "assets.hpp"
#include <fstream>
#include <string>
#include <zlib.h>

#include <filesystem>

std::shared_ptr<uint8_t> read_embedded_data(std::ifstream &&file,
                                            const assets::asset_common &header) {
    if (!header.is_asset_embedded)
        return nullptr;

    const auto origin = file.tellg();
    file.seekg(header.embedded_data_offset);

    std::shared_ptr<uint8_t> data(nullptr);

    if (header.is_data_compressed) {
        try {
        auto* ptr = new uint8_t[header.data_decompressed_length];
        data.reset(ptr);
        } catch (const std::bad_alloc& ex) {
            return nullptr;
        }

        auto *buf = new uint8_t[header.embedded_data_length];
        file.read((char *) buf,
                  header.embedded_data_length);

        uncompress(
                data.get(), (uLongf *) &header.data_decompressed_length,
                buf, header.embedded_data_length
        );

        delete[] buf;
    } else {
        data.reset(new uint8_t[header.embedded_data_length]);
        file.read((char *) data.get(),
                  header.embedded_data_length);
    }

    if (file.eof())
        file.clear();
    file.seekg(origin);

    return data;

}

bool write_embedded_data(std::ofstream &file,
                         assets::asset_common &header,
                         const std::shared_ptr<uint8_t> &data) {
    if (!header.is_asset_embedded)
        return false;

    const auto origin = file.tellp();


    static uint32_t data_offset = assets::PAK_DATA_SECTOR_ADDR;
    file.seekp(data_offset);

    uLongf embedded_crc;
    uLongf decompressed_crc = {
            crc32(0, data.get(), header.data_decompressed_length)
    };

    if (header.is_data_compressed) {
        uLongf compressedLength = header.embedded_data_length;
        if (compressedLength == 0)
            compressedLength = compressBound(header.data_decompressed_length);

        auto *compressedBuf = new uint8_t[compressedLength];
        compress((Bytef *) compressedBuf, &compressedLength,
                 data.get(), header.data_decompressed_length
        );
        embedded_crc = {
                crc32(0, compressedBuf, compressedLength)
        };

        header.embedded_data_length = compressedLength;

        file.write((char *) compressedBuf, compressedLength);
        delete[] compressedBuf;
    } else {
        embedded_crc = decompressed_crc;
        file.write((char *) data.get(), header.embedded_data_length);
    }


    header.embedded_data_offset = data_offset;
    header.crc_decompressed = decompressed_crc;
    header.crc_embedded = embedded_crc;
    data_offset += header.embedded_data_length + 4;

    file.seekp(origin);
    return true;
}

std::shared_ptr<assets::PakAsset> read_pak_asset(std::ifstream &file,
                                                 bool processData = true) {
    std::shared_ptr<assets::PakAsset> asset(new assets::PakAsset{});
    file.read((char *) &asset->header,
              sizeof(assets::pak_asset_header));

    if (processData && asset->header.common.is_asset_embedded)
        asset->data = read_embedded_data(std::move(file), asset->header.common);
    return asset;
}


bool write_pak_asset(std::ofstream &file,
                     const std::shared_ptr<assets::PakAsset> &asset,
                     bool processData = true) {
    if (processData && asset->header.common.is_asset_embedded)
        write_embedded_data(file, asset->header.common, asset->data);

    file.write((char *) &asset->header,
               sizeof(assets::pak_asset_header));
    return true;
}


void assets::AssetsIO::read_pak_contents(const std::string &path,
                                         assets::PakContents &contents) {
    contents.assets.clear();
    std::ifstream file(
            path, std::ios::binary
    );
    if (file.fail())
        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory),
                                "File not found.");


    {
        file.read((char *) &contents.fileHeader,
                  sizeof(pak_file_header)); // read assets file header
    }
    if (file.fail())
        throw std::system_error(std::make_error_code(std::errc::invalid_argument),
                                "File header couldn't be read.");

    {
        file.seekg(PAK_ASSET_HEADER_ADDR);
        file.read((char *) &contents.contentHeader,
                  sizeof(pak_content_header)); // read assets content header
    }
    if (file.fail()) {
        throw std::system_error(std::make_error_code(std::errc::invalid_argument),
                                "Content header couldn't be read.");
    }

    contents.assets.reserve(contents.contentHeader.asset_count);
    for (int assetIndex = 0; assetIndex < contents.contentHeader.asset_count; ++assetIndex) {
        auto asset = read_pak_asset(file, true);
        contents.assets.push_back(
                asset
        );
    }

    file.close();
}

void assets::AssetsIO::write_pak_contents(const std::string &path,
                                          assets::PakContents &contents) {
    std::ofstream file(
            path, std::ios::binary
    );
    if (file.fail())
        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory),
                                "Couldn't open file for reading.");

    {
        contents.contentHeader.asset_count = contents.assets.size();
        file.seekp(PAK_ASSET_HEADER_ADDR);
        file.write((char *) &contents.contentHeader,
                   sizeof(pak_content_header)); // write assets content header
    }
    if (file.fail())
        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory),
                                "Couldn't write content header");

    size_t totalAssetCount = 0;
    size_t usedAssetCount = 0;
    size_t delAssetCount = 0;

    size_t compressedAssetCount = 0;
    size_t embeddedAssetCount = 0;

    for (int assetIndex = 0; assetIndex < contents.assets.size(); ++assetIndex) {
        const auto &asset = contents.assets[assetIndex];

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

    file.write((char *) &contents.dataHeader,
               sizeof(pak_data_header)); // write assets data header
    if (file.fail())
        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory),
                                "Couldn't write data header"
        );

    file.seekp(0, std::ios_base::end);
    const auto file_size = file.tellp();

    contents.fileHeader.file_size = file_size;
    contents.fileHeader.asset_count = totalAssetCount;
    contents.fileHeader.used_assets_count = usedAssetCount;
    contents.fileHeader.deleted_assets_count = delAssetCount;

    {
        file.seekp(std::ios_base::beg);
        file.write((char *) &contents.fileHeader,
                   sizeof(pak_file_header)); // write assets file header
    }
    if (file.fail())
        throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory),
                                "Couldn't write file header"
        );

    file.close();
}

bool assets::AssetsIO::export_pak_asset(const std::string &parent_dir,
                                        const std::shared_ptr<PakAsset> &asset) {

    char assetPath[512]{0};
    for (int i = 0; i < 256; ++i) {
        wchar_t o = asset->header.path[i];
        if (o == 0)
            break;
        assetPath[i] = (char) o;
    }

    const auto path = std::filesystem::path(parent_dir + "/" + assetPath);
    const auto parentPath = path.parent_path();

    std::filesystem::create_directories(parentPath);
    std::ofstream file(path,
                       std::ios_base::binary);

    if (asset->header.common.is_data_compressed)
        file.write((char *) asset->data.get(), asset->header.common.data_decompressed_length);
    else
        file.write((char *) asset->data.get(), asset->header.common.embedded_data_length);

    file.close();
    return false;
}