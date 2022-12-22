#include "../../headers/protocol_states.hpp"

using namespace protocol::states;

___impl::ProtocolSMContext::ProtocolSMContext(ProtocolSM* sm) :
    s_idle(sm),
    s_reading_header(sm),
    s_reading_body(sm),
    s_error(sm),
    s_waiting_ack(sm),
    sm(sm)
{}

// Syntax sugar operators
ProtocolSM& protocol::states::operator<<(ProtocolSM& protocol_sm, const protocol::serial::Serializable& serializable) {
    auto& buffer = *protocol_sm.ctx.buffer;
    // Prepare to send message (with header)
    messages::Header header;

    primitives::Checksum calculated_checksum = (uahruart::utils::calculate_hash(
                                        buffer.beginning_tx(),
                                        header.size() + serializable.size()));

    header.length = serializable.size();
    buffer  << header 
            << serializable
            << calculated_checksum;
    // Calculate hash

    return protocol_sm;
}

const ProtocolSM& protocol::states::operator>>(const ProtocolSM& protocol_sm, protocol::serial::Serializable& serializable) {
    auto& buffer = *protocol_sm.ctx.buffer;
    // Prepare to receive message (with header)
    buffer >> serializable;
    return protocol_sm;
}

void ProtocolBuffer::on(Actions action, functor<void ()> callback) {
    if (action <= Actions::SIZE)
        m_callbacks[action] = callback;
}

