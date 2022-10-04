#include "../headers/messages.hpp"

#include "../headers/utils.hpp"

using namespace protocol::messages;

// ==================================================
// Header
// ==================================================

#include <iostream>
void Header::serialize(protocol::base128::Stream& stream) const {

}

void Header::deserialize(protocol::base128::Stream& stream) {

}

// ==================================================
// Admin
// ==================================================
void Admin::serialize(protocol::base128::Stream& stream) const {
    stream << ack;
} 

void Admin::deserialize(protocol::base128::Stream& stream) {
    stream >> ack;
}

// ==================================================
// RMI
// ==================================================
