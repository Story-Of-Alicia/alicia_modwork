//
// Created by rgnt on 10/24/2020.
//
#include "pak.hpp"

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

void MagicPrint(const char *format, int arg) {
    printf(format, arg, arg);
}

/**
* Used for generating report.html which can be displayed in web browser.
**/
void PrintAssetHTML(uint64_t index, const PakAsset &asset, uint64_t loc) {
    printf("\n");
    printf("<div class=\"asset\">\n");
    printf("<p><span class=\"index\">#%llu</span> %ls <span class=\"loc\">[<span class=\"hex\">0x%llX]</span></span> %s </p>\n", index,
           (asset.path[0] == 0 ? asset.path.c_str() : (L"<span class=\"red\">(missing path)</span>")),
           loc, asset.isDeleted ? "<span class=\"red\">(deleted)</span>" : "");
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

    MagicPrint(
            "<tr>\n<td>\nPrefix</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.prefix);
    MagicPrint(
            "<tr>\n<td>\nMagic</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.magic);
    MagicPrint(
            "<tr>\n<td>\nData Offset</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.dataOffset);
    MagicPrint(
            "<tr>\n<td>\nData Length</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.dataLength);

    MagicPrint(
            "<tr>\n<td>\nUncompressed Length0</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.uncompressedLength0);
    MagicPrint(
            "<tr>\n<td>\nIs compressed</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.isCompressed);
    MagicPrint(
            "<tr>\n<td>\nUncompressed Length1</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.uncompressedLength1);
    MagicPrint(
            "<tr>\n<td>\nUnknown0</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.unknown0);
    MagicPrint(
            "<tr>\n<td>\nUncompressed Length2</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.uncompressedLength2);

    MagicPrint(
            "<tr>\n<td>\nUnknown1</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.unknown1);
    MagicPrint(
            "<tr>\n<td>\nUnknown2</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.unknown2);
    MagicPrint(
            "<tr>\n<td>\nUnknown3</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.unknown3);
    MagicPrint(
            "<tr>\n<td>\nUnknown4</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.unknown4);
    MagicPrint(
            "<tr>\n<td>\nUnknown5</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.unknown5);
    MagicPrint(
            "<tr>\n<td>\nIs Deleted</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.isDeleted);
    MagicPrint(
            "<tr>\n<td>\nEntry offset</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.entryOffset);
    MagicPrint(
            "<tr>\n<td>\nIs packed</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.isPacked);
    MagicPrint(
            "<tr>\n<td>\nUnknownType</td>\n<td>\n<code>long</code></td>\n<td>\n%llu<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.unknownType);
    MagicPrint(
            "<tr>\n<td>\nUnknownValue</td>\n<td>\n<code>long</code></td>\n<td>\n%llu<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.unknownValue);
    MagicPrint(
            "<tr>\n<td>\nCRC Result</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.crcResult);
    MagicPrint(
            "<tr>\n<td>\nUnknown9</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.unknown9);
    MagicPrint(
            "<tr>\n<td>\nCrcId</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.crcId);

    MagicPrint(
            "<tr>\n<td>\nUnknown7</td>\n<td>\n<code>int</code></td>\n<td>\n%u<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.unknown7);
    MagicPrint(
            "<tr>\n<td>\nUnknown8</td>\n<td>\n<code>long</code></td>\n<td>\n%llu<span class=\"hex\">(0x%X)</span></td>\n<td>\n</tr>\n",
            asset.unknown8);

    printf("</table></div>\n");

    printf("\n");
}


uint64_t WriteToFile(std::ofstream &file, uint8_t* data, uint64_t length, uint64_t decompressedLength, bool decompress) {
    if (decompress) {
        // write uncompressed
        uLongf uncompressed = decompressedLength;
        auto *dest = new uint8_t[uncompressed];

        uncompress(dest, &uncompressed, (Bytef*) data, length);
        if(uncompressed)
            file.write((char *) dest, uncompressed * sizeof(uint8_t));
        delete[] dest;
        return uncompressed;
    } else {
        // write normal
        file.write((char *) data, length * sizeof(uint8_t));
        return length;
    }
}

void PakFile::Explore() {

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
                .isDeleted = buf.ReadUnsignedInt(),
                .entryOffset = buf.ReadUnsignedInt(),
                .isPacked = buf.ReadUnsignedInt(),

                .unknownType = buf.ReadUnsignedLong(),
                .unknownValue = buf.ReadUnsignedLong(),

                .crcResult = buf.ReadUnsignedInt(),
                .unknown9 = buf.ReadUnsignedInt(),

                .crcId = buf.ReadUnsignedInt(),
                .unknown7 = buf.ReadUnsignedInt(),
                .unknown8 = buf.ReadUnsignedLong(),

                .path = buf.ReadWideString(258)
        };

        if (asset.unknownType == 0) {
            break;
        }


        printf("#%d - %s", count + 1, asset.HasPath() ? asset.GetStandardPath() : "(no path)...");

        if (asset.isPacked) {
            printf(" Packed...");
            uint64_t temp = ftell(this->fileHandle);
            fseek(this->fileHandle, asset.dataOffset, SEEK_SET);

            uLongf length = asset.dataLength;
            auto *src = new uint8_t[length];
            fread(src, sizeof(uint8_t), asset.dataLength, this->fileHandle);

            char *cPath;
            if(asset.HasPath())
                cPath = asset.GetStandardPath();
            else
                cPath = new char[512];

            if (!asset.isDeleted) {
                char directory[128];
                _splitpath(cPath, nullptr, directory, nullptr, nullptr);
            	
                // printf("Dir: %s | Path: %s\n", dirs, cPath);
                std::filesystem::create_directories(directory);

            } else {
                std::filesystem::create_directory("deleted/");
                sprintf(cPath, "deleted/deleted#%llu.bin", count+1);
                printf(" Deleted...");
            }

            if (asset.isCompressed)
                printf(" Compressed...");

            std::ofstream file(cPath, std::ios::binary);
            if (!asset.isDeleted)  
                WriteToFile(file, src, length, asset.uncompressedLength0, asset.isCompressed);
            else {
                // if uncompressed size is bad, write normal
                if(!WriteToFile(file, src, length, asset.uncompressedLength0, true))
                    WriteToFile(file, src, length, length, false);
            }

            //PrintAssetHTML(count + 1, asset, assetLoc);
            //printf("\tDone!\n");

        	
            delete[] src;
            delete (cPath);
            fseek(this->fileHandle, temp, SEEK_SET);
        }
        else
            printf(" Not packed! Skip!");

        printf(" Done!\n");
    	
        count++;
    }

    printf("%llu\n", count);
}


void PakFile::Destroy() {

}
