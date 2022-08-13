#include "messages.hpp"

using namespace protocol::messages;

void Header::serialize(char *buffer) const {
    buffer << len << header_checksum << message_checksum;
}

void Header::deserialize(const char *buffer) {
    buffer >> len >> header_checksum >> message_checksum;
}
