
//
// Created by maros on 10/24/2020.
//

#ifndef ALICIACPAK_BUFFER_HPP
#define ALICIACPAK_BUFFER_HPP


#include <cstdint>
#include <cstdio>
#include <signal.h>
#include <malloc.h>

/**
 * Indirect buffer
 */
class Buffer {
protected:
    uint8_t *data;
    uint64_t dataLength;
    uint64_t dataCursor = 0;

public:
    /**
     * Creates buffer with data
     * @param data       Data
     * @param dataLength Data length in bytes
     */
    Buffer(uint8_t *data, uint64_t dataLength);
    ~Buffer();
public:
    /**
     * Reads next signed byte
     * @return Signed byte
     */
    int8_t ReadByte();

    /**
     * Reads next unsigned byte
     * @return Unsigned byte
     */
    uint8_t ReadUnsignedByte();

    /**
     * Reads next signed short
     * @return Signed short
     */
    int16_t ReadShort();

    /**
     * Reads next unsigned short
     * @return Unsigned short
     */
    uint16_t ReadUnsignedShort();

    /**
     * Reads next signed int
     * @return Signed int
     */
    int32_t ReadInt();

    /**
     * Reads next unsigned int
     * @return Unsigned int
     */
    uint32_t ReadUnsignedInt();

    /**
     * Reads next signed long
     * @return Signed long
     */
    int64_t ReadLong();

    /**
     * Reads next unsigned long
     * @return Unsigned long
     */
    uint64_t ReadUnsignedLong();

    /**
     * Reads next wide string
     * @param maxLength Max length of wide string
     * @return Wide string
     */
    wchar_t* ReadWideString(int maxLength);

    /**
     * Reads next string
     * @param maxLength Max length of string
     * @return String
     */
    char* ReadString(int maxLength);


public:
    /**
     * Resets cursor position
     */
    void ResetCursor() {
        this->dataCursor = 0;
    }

    /**
     * Sets cursor position
     * @param cursor Cursor positon
     */
    void SetCursor(uint64_t cursor) {
        this->dataCursor = cursor;
    }

    /**
     * @return Cursor position
     */
    uint64_t GetCursor() const {
        return this->dataCursor;
    }

public:
    void Destroy();

public:
    /**
     * Converts little-endian array of bytes to big-endian number
     * @param bytes Byte array
     * @return Big-endian Unsigned Int(Long)
     */
    static uint32_t ToUnsignedInt(const uint8_t bytes[4]);

    /**
     * Converts little-endian array of bytes to big-endian number
     * @param bytes Byte array
     * @return Big-endian Unsigned Long Long
     */
    static uint64_t ToUnsignedLong(const uint8_t bytes[8]);

    /**
     * Converts little-endian array of bytes to big-endian number
     * @param bytes Byte array
     * @return Big-endian Unsigned Wide Char
     */
    static uint16_t ToUnsignedWideChar(const uint8_t bytes[2]);

    /**
     * Converts big-endian unsigned wide char to signed big-endian char
     * @param bytes Byte array
     * @return Big-endian Signed Char
     */
    static uint8_t ToUnsignedChar(const uint16_t wideChar);

};

/**
 * Direct buffer
 */
class DirectBuffer : Buffer {
private:
    FILE* file;
public:
    DirectBuffer(FILE *file, uint64_t length);

public:
    uint8_t ReadUnsignedByte();
};


#endif //ALICIACPAK_BUFFER_HPP
