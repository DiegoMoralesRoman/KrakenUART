#include "../headers/messages.hpp"

#include "../headers/utils.hpp"

using namespace protocol::messages;

// ==================================================
// Header
// ==================================================

char *const Header::serialize(char *const buffer) const {
    return buffer;
}

char *const Header::deserialize(char *const buffer) {
    return buffer;
}

// ==================================================
// Test message TODO: delete when finished
// ==================================================

char *const Test::serialize(char *const buffer) const {
    return buffer << number;
}

char *const Test::deserialize(char *const buffer) {
    return buffer >> number;
}

// ==================================================
// RMI
// ==================================================
