#include "../headers/primitives.hpp"

#include <stdint.h>
#include <utility>
#include <type_traits>

using namespace protocol::primitives;


// ==================================================
// Initialization
// ==================================================
const Int8 protocol::primitives::TRUE = Int8(0b11111111);
const Int8 protocol::primitives::FALSE = false;

// Global variables
namespace {
    bool is_big_endian = false;
}

const bool ___init = []() -> bool {
    // Check if using big endian
    uint32_t n = 1;
    //*const_cast<bool*>(&is_big_endian) = *reinterpret_cast<char*>(&n) == 1;
    is_big_endian = *reinterpret_cast<char*>(&n) == 1;
    return true;
}();


// ==================================================
// Primitive serialization implementation
// ==================================================
void Int32::serialize(protocol::serial::Stream& stream) const {
    if (is_big_endian) {
        stream.write(reinterpret_cast<const char*>(&m_value), 4);
    } else {
        auto* tmp = reinterpret_cast<const char*>(&m_value);
        char swap[4];
        swap[0] = tmp[3];
        swap[1] = tmp[2];
        swap[2] = tmp[1];
        swap[3] = tmp[0];
        stream.write(static_cast<const char*>(swap), 4);
    }
}

void Int32::deserialize(protocol::serial::Stream& stream) {
    if (is_big_endian)
        stream.read(reinterpret_cast<char*>(&m_value), 4);
    else {
        auto* tmp = reinterpret_cast<char*>(&m_value);
        char buffer[4];
        stream.read(buffer, 4);
        tmp[0] = buffer[3];
        tmp[1] = buffer[2];
        tmp[2] = buffer[1];
        tmp[3] = buffer[0];
    }
}

// Int16
void Int16::serialize(protocol::serial::Stream& stream) const {
    if (is_big_endian)
        stream.write(reinterpret_cast<const char*>(&m_value), 2);
    else {
        auto* tmp = reinterpret_cast<const char*>(&m_value);
        char swap[2];
        swap[0] = tmp[1];
        swap[1] = tmp[0];
        stream.write(static_cast<const char*>(swap), 2);
    }
}

void Int16::deserialize(serial::Stream &stream) {
    if (is_big_endian) {
        stream.read(reinterpret_cast<char*>(&m_value), 2);
    } else {
        auto* tmp = reinterpret_cast<char*>(&m_value);
        char buffer[2];
        stream.read(buffer, 2);
        tmp[0] = buffer[1];
        tmp[1] = buffer[0];
    }
}

// // Int8
void Int8::serialize(serial::Stream &stream) const {
    stream.write(reinterpret_cast<const char*>(&m_value), 1);
}

void Int8::deserialize(serial::Stream &stream) {
    stream.read(reinterpret_cast<char*>(&m_value), 1);
}

void String::serialize(serial::Stream &stream) const {
    stream << Size(string.length());
    stream.write(string.c_str(), string.length());
}

void String::deserialize(serial::Stream &stream) {
    Size len;
    stream >> len;
    string = std::string(static_cast<size_t>(len), 0);
    stream.read(string.data(), string.length());
}

String::String(const char* str)
    : string(str) {}
