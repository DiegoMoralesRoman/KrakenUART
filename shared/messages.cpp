#include "messages.hpp"

#include "utils.hpp"

using namespace protocol::messages;

// ==================================================
// Header
// ==================================================

uint32_t Header::get_checksum() const {
    return uahruart::utils::hash_uint32(len) ^ uahruart::utils::hash_uint32(static_cast<uint32_t>(type()));
}

char* Header::serialize(char *buffer) const {
    // Calculate checksum
    header_checksum = get_checksum();
    return buffer << len << header_checksum << primitives::Int8(type());
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
    buffer = Header::serialize(buffer);
    return buffer << ack;
} 

const char* Admin::deserialize(const char* buffer) {
    buffer = Header::deserialize(buffer); 
    return buffer >> ack;
}

