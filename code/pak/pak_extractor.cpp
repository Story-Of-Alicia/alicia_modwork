/**
* Created by rgnt.xyz
* Reports all resources in PAK file. Result will be written in report.txt
*/

#include <cstdio>

#include <unistd.h>
#include <stdint.h>
#include <malloc.h>

uint32_t ToUnsignedInt(const uint8_t bytes[4]) {
    return ((uint32_t) bytes[3]) << 24u | ((uint32_t) bytes[2]) << 16u | ((uint32_t) bytes[1]) << 8u | bytes[0];
}

uint64_t ToUnsignedLong(const uint8_t bytes[8]) {
    return
            ((uint64_t) bytes[7]) << 54u | ((uint64_t) bytes[6]) << 48u | ((uint64_t) bytes[5]) << 40u |
            ((uint64_t) bytes[4]) << 32u |
            ((uint64_t) bytes[3]) << 24u | ((uint64_t) bytes[2]) << 16u | ((uint64_t) bytes[1]) << 8u |
            ((uint64_t) bytes[0]);
}

uint16_t ToUnsignedWideChar(const uint8_t bytes[2]) {
    return ((uint16_t) bytes[1]) << 8u | ((uint16_t) bytes[0]);
}

uint8_t ToUnsignedChar(const uint16_t wideChar) {
    return wideChar >> 8u;
}

struct RBuffer {
private:
    uint32_t index = 0;
    uint32_t length;
    const uint8_t *data;

public:
    RBuffer(uint32_t length, const uint8_t *data) : length(length) {
        this->data = data;
    }

private:
    uint8_t GetAndIncrement() {
        if (index > length) {
            printf("Reader index greater than data length\n");
            exit(1);
        }

        return data[index++];
    }

public:
    uint32_t ReadNextUInt() {
        uint8_t bytes[4]{GetAndIncrement(), GetAndIncrement(), GetAndIncrement(), GetAndIncrement()};

        return ToUnsignedInt(bytes);
    }

    uint64_t ReadNextULong() {
        uint8_t bytes[8]{GetAndIncrement(), GetAndIncrement(), GetAndIncrement(), GetAndIncrement(),
                         GetAndIncrement(), GetAndIncrement(), GetAndIncrement(), GetAndIncrement()};

        return ToUnsignedLong(bytes);
    }

    uint16_t ReadNextUWideChar() {
        uint8_t bytes[2]{GetAndIncrement(), GetAndIncrement()};

        return ToUnsignedWideChar(bytes);
    }

    uint16_t *ReadNextWideString(int maxLength) {
        auto *str = (uint16_t *) calloc(maxLength, sizeof(uint16_t));

        for (int i = 0; i < maxLength; i++) {
            str[i] = ReadNextUWideChar();
        }

        return str;
    }

};

struct PakRes {
public:
    uint32_t stackIndicator;

    uint32_t resourceOffset;
    uint32_t resourceLength;

    uint64_t fileLength0;
    uint64_t fileLength1;
    uint32_t unknown0;

    uint32_t unknown1;
    uint32_t unknown2;
    uint32_t unknown3;
    uint32_t unknown4;
    uint32_t unknown5;
    uint32_t unknown6;
    uint32_t unknown7;

    uint64_t fisType;
    uint32_t fisValue;

    uint32_t unknown8;
    uint32_t unknown9;
    uint32_t unknown10;

    uint32_t crcIdentification;
    uint32_t crcValue;
    uint64_t unknown11;
    uint32_t unknown12;

    uint16_t *path;

    uint8_t data[];

public:
    void Destroy() {
        // free(this->data);
    }

    bool IsEncoded() const {
        return resourceOffset != 0;
    }

    uint64_t GetLength() const {
        if (IsEncoded())
            return this->resourceLength;
        else
            return this->fileLength0;
    }

    const uint16_t *GetPath() const {
        return this->path;
    }


};

FILE *GetFileHandle(char *path, const char *access) {
    FILE *file;
    fopen_s(&file, path,
            access);
    return file;
}

PakRes ReadResource(uint8_t *bytes) {
    RBuffer rbuf(620, bytes);
    return PakRes{
            .stackIndicator = rbuf.ReadNextUInt(),
            .resourceOffset = rbuf.ReadNextUInt(),
            .resourceLength = rbuf.ReadNextUInt(),

            .fileLength0    = rbuf.ReadNextULong(),
            .fileLength1    = rbuf.ReadNextULong(),

            .unknown0       = rbuf.ReadNextUInt(),
            .unknown1       = rbuf.ReadNextUInt(),
            .unknown2       = rbuf.ReadNextUInt(),
            .unknown3       = rbuf.ReadNextUInt(),
            .unknown4       = rbuf.ReadNextUInt(),
            .unknown5       = rbuf.ReadNextUInt(),
            .unknown6       = rbuf.ReadNextUInt(),
            .unknown7       = rbuf.ReadNextUInt(),

            .fisType        = rbuf.ReadNextULong(),
            .fisValue       = rbuf.ReadNextUInt(),

            .unknown8       = rbuf.ReadNextUInt(),
            .unknown9       = rbuf.ReadNextUInt(),
            .unknown10      = rbuf.ReadNextUInt(),

            .crcIdentification = rbuf.ReadNextUInt(),
            .crcValue          = rbuf.ReadNextUInt(),
            .unknown11         = rbuf.ReadNextULong(),
            .unknown12         = rbuf.ReadNextUInt(),

            .path              = rbuf.ReadNextWideString(258)
    };
}

int main() {
    printf("Enter path to RES file: ");
    char path[256];
    scanf("%s", path);

    auto file = GetFileHandle(path, "rb");
    auto log   = GetFileHandle("report.txt", "w");
    printf("File open; READ BINARY\n");

    const int RES_OFFSET = 0xF00000;
    const int ENTRY_OFFSET = 0x7D000;
    int currentOffset = ENTRY_OFFSET + 16;
    fseek(file, currentOffset, SEEK_SET);

    int entryCount = 0;

    int bufferIndex = 0;
    uint8_t buffer[620] = {0};

    for (; currentOffset < RES_OFFSET; currentOffset++) {
        uint8_t byte = getc(file);
        buffer[bufferIndex++] = byte;

        if (bufferIndex == 620) {
            // stack indicator cant be null
            if(buffer[0] != 0) {
                  PakRes resource = ReadResource(buffer);
                  fprintf(log, "Offset: %-10X Length: %-10X ", resource.resourceOffset, resource.GetLength());
                  fprintf(log, "Path: %ls\n", resource.path);
                  entryCount++;
            }
            bufferIndex = 0;
        }

    }
    
    fclose(file);
    fclose(log);


    return 0;
}
