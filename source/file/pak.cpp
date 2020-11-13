//
// Created by maros on 10/24/2020.
//
#include <zlib.h>
#include <boost/filesystem.hpp>
#include <fstream>

#include "pak.hpp"

namespace file = boost::filesystem;

PakFile::PakFile(const char *path) {
    fopen_s(&this->fileHandle, path, "rb");
    if (!this->fileHandle) {
        printf("File not found!\n");
        raise(SIGABRT);
        return;
    } else
        this->isOk = true;
}

PakFile::~PakFile() {
    Destroy();
}

void PrintHTMLAsset(uint64_t index, const PakAsset &asset, uint64_t loc) {
    printf("\n");
    printf("<div class=\"asset\">\n");
    printf("<p><span class=\"index\">#%llu</span> %ls <span class=\"loc\">[0x%llX]</span></p>\n", index, asset.path.c_str(), loc);
    printf("<table class=\"table table-striped table-dark\">\n");
    printf("<tr><th>\n");
    printf("Field Name\n");
    printf("</th>\n");
    printf("<th>\n");
    printf("Field Type\n");
    printf("</th>\n");
    printf("<th>\n");
    printf("Field Value\n");
    printf("</th></tr>\n");

    printf("<tr>\n<td>\nPrefix</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.prefix);
    printf("<tr>\n<td>\nMagic</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.magic);
    printf("<tr>\n<td>\nData Offset</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.dataOffset);
    printf("<tr>\n<td>\nData Length</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.dataLength);

    printf("<tr>\n<td>\nUncompressed Length0</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.uncompressedLength0);
    printf("<tr>\n<td>\nIs compressed</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.isCompressed);
    printf("<tr>\n<td>\nUncompressed Length1</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.uncompressedLength1);
    printf("<tr>\n<td>\nUnknown0</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.unknown0);
    printf("<tr>\n<td>\nUncompressed Length2</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.uncompressedLength2);

    printf("<tr>\n<td>\nUnknown1</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.unknown1);
    printf("<tr>\n<td>\nUnknown2</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.unknown2);
    printf("<tr>\n<td>\nUnknown3</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.unknown3);
    printf("<tr>\n<td>\nUnknown4</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.unknown4);
    printf("<tr>\n<td>\nUnknown5</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.unknown5);
    printf("<tr>\n<td>\nUnknown6</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.unknown6);
    printf("<tr>\n<td>\nUnknown7</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.entryOffset);
    printf("<tr>\n<td>\nIs packed</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.isPacked);
    printf("<tr>\n<td>\nUnknownType</td>\n<td>\n<code>long</code></td>\n<td>\n%llu</td>\n<td>\n</tr>\n",asset.unknownType);
    printf("<tr>\n<td>\nUnknownValue</td>\n<td>\n<code>long</code></td>\n<td>\n%llu</td>\n<td>\n</tr>\n",asset.unknownValue);
    printf("<tr>\n<td>\nUnknown9</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.unknown8);
    printf("<tr>\n<td>\nUnknown10</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.unknown9);
    printf("<tr>\n<td>\nCrcId</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.crcId);
    printf("<tr>\n<td>\nCrcValue</td>\n<td>\n<code>int</code></td>\n<td>\n%u</td>\n<td>\n</tr>\n",asset.crcValue);
    printf("<tr>\n<td>\nUnknown11</td>\n<td>\n<code>long</code></td>\n<td>\n%llu</td>\n<td>\n</tr>\n",asset.unknown10);
    printf("</table></div>\n");

    printf("\n");

}

void PakFile::LoadFromDisk() {

    fseek(this->fileHandle, PakFile::ENTRY_SECTOR + 12, SEEK_SET);

    uint64_t count = 0;
    for (;;) {
        uint8_t data[620];
        auto assetLoc = ftell(this->fileHandle);
        fread(data, sizeof(uint8_t), 620, this->fileHandle);

        Buffer buf(data, 620);
        PakAsset asset{
                .prefix = buf.ReadUnsignedInt(),
                .magic = buf.ReadUnsignedInt(),

                .dataOffset = buf.ReadUnsignedInt(),
                .dataLength = buf.ReadUnsignedInt(),

                .uncompressedLength0 = buf.ReadUnsignedInt(),
                .isCompressed = buf.ReadUnsignedInt(),
                .uncompressedLength1 = buf.ReadUnsignedInt(),
                .unknown0 = buf.ReadUnsignedInt(),
                .uncompressedLength2 = buf.ReadUnsignedInt(),

                .unknown1 = buf.ReadUnsignedInt(),
                .unknown2 = buf.ReadUnsignedInt(),
                .unknown3 = buf.ReadUnsignedInt(),
                .unknown4 = buf.ReadUnsignedInt(),
                .unknown5 = buf.ReadUnsignedInt(),
                .unknown6 = buf.ReadUnsignedInt(),
                .entryOffset = buf.ReadUnsignedInt(),
                .isPacked = buf.ReadUnsignedInt(),

                .unknownType = buf.ReadUnsignedLong(),
                .unknownValue = buf.ReadUnsignedLong(),

                .unknown8 = buf.ReadUnsignedInt(),
                .unknown9 = buf.ReadUnsignedInt(),

                .crcId = buf.ReadUnsignedInt(),
                .crcValue = buf.ReadUnsignedInt(),
                .unknown10 = buf.ReadUnsignedLong(),

                .path = buf.ReadWideString(258)
        };
        if (asset.unknownType == 0)
            break;

        /*if(asset.isCompressed && asset.path.find(L".dff") != std::wstring::npos) {
            uint64_t temp = ftell(this->fileHandle);
            printf("#%llu - %ls\n", count+1, asset.path.c_str());
            fseek(this->fileHandle, asset.dataOffset, SEEK_SET);

            uLongf uncompressed = asset.uncompressedLength0;
            uLongf compressed   = asset.dataLength;

            auto *dest = (uint8_t*) calloc(uncompressed, sizeof(uint8_t));
            auto *src = (uint8_t*) calloc(compressed, sizeof(uint8_t));

            fread(src, sizeof(uint8_t), compressed, this->fileHandle);

            int result = uncompress(dest, &uncompressed, src, compressed);
            if(result == Z_OK) {
                FILE* aa = fopen("alicia.dff", "w+");
                fwrite(dest, sizeof(uint8_t), uncompressed, aa);
                fclose(aa);
                printf("file\n");
            }

            if(result == Z_DATA_ERROR) {
                printf("Corrupted!\n");
            }
            */

        if(asset.path.ends_with(L".png"))  {
            uint64_t temp = ftell(this->fileHandle);

            uint8_t src[asset.dataLength];
            fread(src, sizeof(uint8_t), asset.dataLength, this->fileHandle);

            std::ofstream file(asset.path.c_str());
            if(file::exists(asset.path.c_str())){
                printf("exists\n");
            }

            file << src;
            if(file.bad())
                printf("open\n");


            break;
            fseek(this->fileHandle, temp, SEEK_SET);
        }
        //PrintHTMLAsset(count + 1, asset, assetLoc);

        count++;
    }
    printf("%llu\n", count);
}


void PakFile::Destroy() {

}
