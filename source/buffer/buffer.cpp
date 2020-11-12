//
// Created by maros on 10/24/2020.
//

#include <cstdlib>
#include "buffer.hpp"


Buffer::Buffer(uint8_t *data, uint64_t dataLength) : data(data), dataLength(dataLength) {}

uint32_t Buffer::ToUnsignedInt(const uint8_t bytes[4]) {
    return
            ((uint32_t) bytes[3]) << 24u | ((uint32_t) bytes[2]) << 16u | ((uint32_t) bytes[1]) << 8u | bytes[0];
}

uint64_t Buffer::ToUnsignedLong(const uint8_t bytes[8]) {
    return
            ((uint64_t) bytes[7]) << 54u | ((uint64_t) bytes[6]) << 48u | ((uint64_t) bytes[5]) << 40u |
            ((uint64_t) bytes[4]) << 32u |
            ((uint64_t) bytes[3]) << 24u | ((uint64_t) bytes[2]) << 16u | ((uint64_t) bytes[1]) << 8u |
            ((uint64_t) bytes[0]);
}

uint16_t Buffer::ToUnsignedWideChar(const uint8_t bytes[2]) {
    return
            ((uint16_t) bytes[1]) << 8u | ((uint16_t) bytes[0]);
}

uint8_t Buffer::ToUnsignedChar(const uint16_t wideChar) {
    return wideChar >> 8u;
}


int8_t Buffer::ReadByte() {
    return this->ReadUnsignedByte();
}

uint8_t Buffer::ReadUnsignedByte() {
    if(dataLength == dataCursor) {
        printf("MAX LENGTH REACHED %llu==%llu\n", dataLength, dataCursor);
        abort();
    }

    return data[dataCursor++];
}


int16_t Buffer::ReadShort() {
    return ReadUnsignedShort();
}

uint16_t Buffer::ReadUnsignedShort() {
    return ReadUnsignedByte() + ((uint16_t)ReadUnsignedByte() << 8u);
}

int32_t Buffer::ReadInt() {
    return ReadUnsignedInt();
}

uint32_t Buffer::ReadUnsignedInt() {
    return ReadUnsignedShort() + ((uint32_t)ReadUnsignedShort() << 16u);
}

int64_t Buffer::ReadLong() {
    return ReadUnsignedLong();
}

uint64_t Buffer::ReadUnsignedLong() {
    return ReadUnsignedInt() + ((uint64_t) ReadUnsignedInt() << 32u);
}

wchar_t* Buffer::ReadWideString(int maxLength) {
    auto* str = new wchar_t [maxLength]{0};

    for(int i = 0; i < maxLength; i++) {
        uint8_t byte = ReadUnsignedShort();
        if(byte == 0x0)
            break;
        str[i] = byte;
    }

    return str;
}

char *Buffer::ReadString(int maxLength) {
    auto* str = new char[maxLength]{0};

    for(int i = 0; i < maxLength; i++) {
        uint8_t byte = ReadByte();
        if(byte == 0x0)
            break;
        str[i] = byte;
    }

    return str;
}

void Buffer::Destroy() {
    ResetCursor();
    this->dataLength = 0;
}

Buffer::~Buffer() {
    Destroy();
}


uint8_t DirectBuffer::ReadUnsignedByte() {
    if(dataLength == dataCursor) {
        printf("MAX LENGTH REACHED %llu==%llu\n", dataLength, dataCursor);
        abort();
    }

    return fgetc(file);
}

DirectBuffer::DirectBuffer(FILE *file, uint64_t length) : Buffer(nullptr, length), file(file) {

}
