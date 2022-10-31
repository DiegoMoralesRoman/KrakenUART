#include "../headers/messages.hpp"

#include "../headers/utils.hpp"

using namespace protocol::messages;

// ==================================================
// Header
// ==================================================

#include <iostream>
void Header::serialize(protocol::serial::Stream& stream) const {

}

void Header::deserialize(protocol::serial::Stream& stream) {

}

// ==================================================
// Admin
// ==================================================
void Admin::serialize(protocol::serial::Stream& stream) const {
    stream << ack;
} 

void Admin::deserialize(protocol::serial::Stream& stream) {
    stream >> ack;
}

// ==================================================
// RMI
// ==================================================
