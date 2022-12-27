#include "../../headers/protocol_states.hpp"

using namespace uahruart::protocol::states;

___impl::ProtocolSMContext::ProtocolSMContext(ProtocolSM* sm) :
    s_idle(sm),
    s_reading_header(sm),
    s_reading_body(sm),
    s_error(sm),
    s_waiting_ack(sm),
    sm(sm)
{}

// Syntax sugar operators
ProtocolSM& uahruart::protocol::states::operator<<(ProtocolSM& protocol_sm, const uahruart::protocol::serial::Serializable& serializable) {
    auto& buffer = *protocol_sm.ctx.buffer;
    // Prepare to send message (with header)
    uahruart::protocol::messages::Header header;

    // Fill header information
    header.length = serializable.size();
    header.type = static_cast<uint16_t>(serializable.type_id());

    buffer = buffer  << header 
                     << serializable;

    // Calculate checksum of header and message
    uahruart::protocol::primitives::Checksum calculated_checksum = (uahruart::utils::calculate_hash(
                                        buffer.beginning_tx(),
                                        header.size() + serializable.size()));

    buffer << calculated_checksum;
    // Calculate hash

    return protocol_sm;
}

const ProtocolSM& uahruart::protocol::states::operator>>(const ProtocolSM& protocol_sm, uahruart::protocol::serial::Serializable& serializable) {
    auto& buffer = *protocol_sm.ctx.buffer;
    // Prepare to receive message (with header)
    buffer >> serializable;
    return protocol_sm;
}

void ProtocolBuffer::on(Actions action, functor<void (size_t)> callback) {
    if (action <= Actions::SIZE)
        m_callbacks[action] = callback;
}

