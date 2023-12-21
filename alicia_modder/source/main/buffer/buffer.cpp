//
// Created by maros on 31/05/2021.
//

#include "buffer.hpp"
#include "cstdio"
#include "string.h"

buffer::stack_buffer::stack_buffer(const void *dataPtr, size_t dataLen) :
        m_buffer((uint8_t *) dataPtr), m_bufferLength(dataLen), m_bufferCursor(0) {
}

void buffer::stack_buffer::setCursor(size_t pos) {
    this->m_bufferCursor = pos;
}

void buffer::stack_buffer::resetCursor() {
    this->m_bufferCursor = 0;
}

size_t buffer::stack_buffer::getCursor() const {
    return this->m_bufferCursor;
}

uint8_t *buffer::stack_buffer::dataPtr() const {
    return this->m_buffer;
}

size_t buffer::stack_buffer::getLength() const  {
    return this->m_bufferLength;
}

size_t buffer::stack_buffer::getSize() const {
    return this->m_bufferLength;
}

uint8_t buffer::stack_buffer::readUnsignedByte() {
    if (m_bufferCursor > m_bufferLength)
        return 0;
    const auto result = m_buffer[m_bufferCursor++];
    return result;
}

void buffer::stack_buffer::writeUnsignedByte(uint8_t byte) {
    m_buffer[m_bufferCursor++] = byte;
}

uint8_t buffer::stack_buffer::peekUnsignedByte() const {
    return m_buffer[m_bufferCursor];
}

int8_t buffer::stack_buffer::readByte() {
    return (int8_t) readUnsignedByte();
}

void buffer::stack_buffer::writeByte(int8_t byte) {
    writeUnsignedByte((uint8_t) byte);
}

int8_t buffer::stack_buffer::peekByte() const {
    return (int8_t) peekUnsignedByte();
}



uint16_t buffer::stack_buffer::readUnsignedShort() {
    return (uint16_t) (readUnsignedByte() + (readUnsignedByte() << 8));
}

void buffer::stack_buffer::writeUnsignedShort(uint16_t val) {
    writeUnsignedByte(val & 0xFF);
    writeUnsignedByte(val >> 8);
}

uint16_t buffer::stack_buffer::peekUnsignedShort() const {
    return (uint16_t) (peekUnsignedByte() + (peekUnsignedByte() << 8));
}

int16_t buffer::stack_buffer::readShort() {
    return (int16_t) readUnsignedShort();
}

void buffer::stack_buffer::writeShort(int16_t val) {
    writeUnsignedShort((uint16_t) val);
}

int16_t buffer::stack_buffer::peekShort() const {
    return (int16_t) peekUnsignedShort();
}


uint32_t buffer::stack_buffer::readUnsignedInteger() {
    uint16_t least = readUnsignedShort();
    uint32_t most  = readUnsignedShort() << 16;
    return (uint32_t) (least+most);
}

void buffer::stack_buffer::writeUnsignedInteger(uint32_t val) {
    writeUnsignedShort(val & 0xFFFF);
    writeUnsignedShort(val << 16);
}

uint32_t buffer::stack_buffer::peekUnsignedInteger() const {
    return (uint32_t) (peekUnsignedShort() + (peekUnsignedShort() << 16));
}

int32_t buffer::stack_buffer::readInteger() {
    return (int32_t) readUnsignedInteger();
}

void buffer::stack_buffer::writeInteger(int32_t val) {
    writeUnsignedInteger((uint16_t) val);
}

int32_t buffer::stack_buffer::peekInteger() const {
    return (int32_t) peekUnsignedInteger();
}

