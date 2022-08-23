#include "../headers/messages.hpp"

#include "../headers/utils.hpp"

using namespace protocol::messages;

// ==================================================
// Header
// ==================================================

#include <iostream>
uint32_t Header::get_checksum() const {
    if (len != message_type)
        return uahruart::utils::hash_uint32(len) ^ uahruart::utils::hash_uint32(static_cast<uint32_t>(message_type));
    else
        return (uahruart::utils::hash_uint32(len) * 2) ^ uahruart::utils::hash_uint32(static_cast<uint32_t>(message_type));
}

char* Header::serialize(char *buffer) const {
    // Calculate checksum
    header_checksum = get_checksum();
    return buffer << len << header_checksum << message_type;
}

const char* Header::deserialize(const char *buffer) {
    const char* ret = buffer >> len >> header_checksum >> message_type;
    m_is_valid_checksum = static_cast<uint32_t>(header_checksum) == get_checksum();
    return ret;
}

// ==================================================
// Admin
// ==================================================
char* Admin::serialize(char* buffer) const {
    return buffer << ack;
} 

const char* Admin::deserialize(const char* buffer) {
    return buffer >> ack;
}

