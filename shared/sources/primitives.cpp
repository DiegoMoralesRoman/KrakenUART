#include "../headers/primitives.hpp"

#include <stdint.h>
#include <string_view>
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

// TODO:make more optimized version using word size for bulk copying
void copy_buffer(const char* source, char* destination, size_t ammount) {
    for (size_t i = 0; i < ammount; i++)
        destination[i] = source[i];
}

// ==================================================
// Primitive serialization implementation
// ==================================================
char *const Int32::serialize(char *const buffer) const {
    auto* tmp = reinterpret_cast<const char*>(&m_value);
    if (is_big_endian) {
        copy_buffer(tmp, buffer, 4);
    } else {
        buffer[0] = tmp[3];
        buffer[1] = tmp[2];
        buffer[2] = tmp[1];
        buffer[3] = tmp[0];
    }
    return buffer + 4;
}

char *const Int32::deserialize(char *const buffer) {
    auto* tmp = reinterpret_cast<char*>(&m_value);
    if (is_big_endian)
        copy_buffer(buffer, tmp, 4);
    else {
        tmp[0] = buffer[3];
        tmp[1] = buffer[2];
        tmp[2] = buffer[1];
        tmp[3] = buffer[0];
    }
    return buffer + 4;
}

// Int16
char *const Int16::serialize(char *const buffer) const { 
    auto* tmp = reinterpret_cast<const char*>(&m_value);
    if (is_big_endian)
        copy_buffer(tmp, buffer, 2);
    else {
        buffer[0] = tmp[1];
        buffer[1] = tmp[0];
    }
    return buffer + 2;
}

char *const Int16::deserialize(char *const buffer) {
    auto* tmp = reinterpret_cast<char*>(&m_value);
    if (is_big_endian) {
        copy_buffer(buffer, reinterpret_cast<char*>(&m_value), 2);
    } else {
        buffer[0] = tmp[1];
        buffer[1] = tmp[0];
    }
    return buffer + 2;
}

// // Int8
char *const Int8::serialize(char *const buffer) const {
    buffer[0] = m_value;
    return buffer + 1;
}

char *const Int8::deserialize(char *const buffer) {
    m_value = buffer[0];
    return buffer + 1;
}

char *const String::serialize(char *const buffer) const {
    char* buff_con = Size(string.length()).serialize(buffer);
    copy_buffer(string.c_str(), buff_con, string.length());
    return buff_con + string.length();
}

char *const String::deserialize(char *const buffer) {
    Size len;
    char* buff_conn = len.deserialize(buffer);
    // TODO: add comprobation for maximum string size
    string = std::string(static_cast<size_t>(len), 0);
    copy_buffer(buff_conn, string.data(), string.length());
    return buff_conn + string.length();
}

String::String(const char* str)
    : string(str) {}
