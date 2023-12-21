//
// Created by maros on 31/05/2021.
//

#ifndef ALICIA_MODDER_BUFFER_HPP
#define ALICIA_MODDER_BUFFER_HPP

#include <cstdint>
#include <malloc.h>

namespace buffer
{

enum class alloc_strategy
{
  STACK,
  HEAP
};

/**
     * basic stack_buffer
     */
class stack_buffer
{
private:
  size_t m_bufferCursor = 0;
  size_t m_bufferLength = 0;

  uint8_t* m_buffer;

public:
  /**
   * Constructs stack_buffer containing data pointer and data length.
   *
   * @param dataPtr Data pointer.
   * @param dataLen Data length.
   */
  stack_buffer(const void* dataPtr, size_t dataLen);

public:
  /**
   * Sets cursor position
   * @param pos cursor position
   */
  void setCursor(size_t pos);

  /**
   * Resets cursor to zero position
   */
  void resetCursor();

  /**
   * @return cursor position
   */
  [[nodiscard]] size_t getCursor() const;

public:
  [[nodiscard]] uint8_t* dataPtr() const;

  [[nodiscard]] size_t getLength() const;

  [[nodiscard]] size_t getSize() const;

public:
  /**
   * Reads unsigned byte at current position and increments cursor
   * @return Byte
   */
  [[nodiscard]] uint8_t readUnsignedByte();

  /**
   * Writes unsigned byte at current cursor position and increments cursor
   */
  void writeUnsignedByte(uint8_t byte);

  /**
   * @return unsigned byte at cursor position.
   */
  [[nodiscard]] uint8_t peekUnsignedByte() const;

  /**
   * Reads byte at current position and increments cursor
   * @return Byte
   */
  [[nodiscard]] int8_t readByte();

  /**
   * Writes byte at current cursor position and increments cursor
   */
  void writeByte(int8_t byte);

  /**
   * @return byte at cursor position.
   */
  [[nodiscard]] int8_t peekByte() const;

  /**
   * Reads unsigned short at current position and increments cursor
   * @return Short
   */
  [[nodiscard]] uint16_t readUnsignedShort();

  /**
   * Writes unsigned short at current cursor position and increments cursor
   */
  void writeUnsignedShort(uint16_t Short);

  /**
   * @return unsigned short at cursor position.
   */
  [[nodiscard]] uint16_t peekUnsignedShort() const;

  /**
   * Reads short at current position and increments cursor
   * @return Short
   */
  [[nodiscard]] int16_t readShort();

  /**
   * Writes short at current cursor position and increments cursor
   */
  void writeShort(int16_t Short);

  /**
   * @return short at cursor position.
   */
  [[nodiscard]] int16_t peekShort() const;

  /**
   * Reads unsigned integer at current position and increments cursor
   * @return Short
   */
  [[nodiscard]] uint32_t readUnsignedInteger();

  /**
   * Writes unsigned integer at current cursor position and increments cursor
   */
  void writeUnsignedInteger(uint32_t Short);

  /**
   * @return unsigned integer at cursor position.
   */
  [[nodiscard]] uint32_t peekUnsignedInteger() const;

  /**
   * Reads integer at current position and increments cursor
   * @return Short
   */
  [[nodiscard]] int32_t readInteger();

  /**
   * Writes integer at current cursor position and increments cursor
   */
  void writeInteger(int32_t Short);

  /**
   * @return integer at cursor position.
   */
  [[nodiscard]] int32_t peekInteger() const;
};

} // namespace buffer

#endif //ALICIA_MODDER_BUFFER_HPP
