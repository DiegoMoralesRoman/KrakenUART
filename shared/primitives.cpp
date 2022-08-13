#include "primitives.hpp"

#include <stdint.h>
#include <utility>

using namespace protocol::primitives;

// ==================================================
// Initialization
// ==================================================

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
// Operators

    // Serialization
template<>
___impl::SerializableContinuation<char*>&& ___impl::SerializableContinuation<char*>::operator<<(const Serializable &ser) {
    ser.serialize(buffer + cum_offset);
    cum_offset += ser.size();
    return std::move(*this);
}


___impl::SerializableContinuation<char*> protocol::primitives::operator<<(char* buffer, const Serializable& ser) {
    ser.serialize(buffer);
    return {buffer, ser.size()};
}

    // Deserialization
template<>
___impl::SerializableContinuation<const char*>&& ___impl::SerializableContinuation<const char*>::operator>>(Serializable &ser) {
    ser.deserialize(buffer + cum_offset);
    cum_offset += ser.size();
    return std::move(*this);
}

___impl::SerializableContinuation<const char*> protocol::primitives::operator>>(const char *buffer, Serializable &ser) {
    ser.deserialize(buffer);
    return {buffer, ser.size()};
}

// Int322
void Int32::serialize(char *t_buffer) const {
    if (is_big_endian) {
        *reinterpret_cast<decltype(m_value)*>(t_buffer) = m_value;
    } else {
        auto* tmp = reinterpret_cast<const char*>(&m_value);
        t_buffer[0] = tmp[3];
        t_buffer[1] = tmp[2];
        t_buffer[2] = tmp[1];
        t_buffer[3] = tmp[0];
    }
}

void Int32::deserialize(const char *buffer) {
    if (is_big_endian) {
        m_value = *reinterpret_cast<const decltype(m_value)*>(buffer);
    } else {
        auto* tmp = reinterpret_cast<char*>(&m_value);
        tmp[0] = buffer[3];
        tmp[1] = buffer[2];
        tmp[2] = buffer[1];
        tmp[3] = buffer[0];
    }
}

// Int16
void Int16::serialize(char *t_buffer) const {
    if (is_big_endian) {
       *reinterpret_cast<decltype(m_value)*>(t_buffer) = m_value;
    } else {
        auto* tmp = reinterpret_cast<const char*>(&m_value);
        t_buffer[0] = tmp[1];
        t_buffer[1] = tmp[0];
    }
}

void Int16::deserialize(const char *buffer) {
    if (is_big_endian) {
        m_value = *reinterpret_cast<const decltype(m_value)*>(buffer);
    } else {
        auto* tmp = reinterpret_cast<char*>(&m_value);
        tmp[0] = buffer[1];
        tmp[1] = buffer[0];
    }
}

// Int8
void Int8::serialize(char *t_buffer) const {
    t_buffer[0] = m_value;
}

void Int8::deserialize(const char *t_buffer) {
    m_value = t_buffer[0];
}

// String
    // NOP deleter
struct nop {
    template<typename T>
    void operator() (T const&) const noexcept {}
};

void String::serialize(char *t_buffer) const {
    t_buffer = t_buffer << Size(string.length());
    // Copy string into buffer
    for (size_t i = 0; i < string.length(); i++)
        t_buffer[i] = string[i];
}

void String::deserialize(const char *t_buffer) {
    Size len;
    t_buffer = t_buffer >> len;
    string = std::string(static_cast<uint32_t>(len), '\0');
    // Copy to string
    for (size_t i = 0; i < static_cast<uint32_t>(len); i++)
        string[i] = t_buffer[i];
}

String::String(const char* str)
    : string(str) {}
