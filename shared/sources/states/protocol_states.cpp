#include "../../headers/protocol_states.hpp"

using namespace protocol::states;

___impl::ProtocolSMContext::ProtocolSMContext(ProtocolSM* sm) :
    s_idle(sm),
    s_reading_header(sm),
    s_reading_body(sm),
    sm(sm)
{}

// Syntax sugar operators
ProtocolSM& protocol::states::operator<<(ProtocolSM& protocol_sm, const protocol::serial::Serializable& serializable) {
    auto& buffer = *protocol_sm.ctx.buffer;
    // Prepare to send message (with header)
    buffer << serializable;
    return protocol_sm;
}

const ProtocolSM& protocol::states::operator>>(const ProtocolSM& protocol_sm, protocol::serial::Serializable& serializable) {
    auto& buffer = *protocol_sm.ctx.buffer;
    // Prepare to receive message (with header)
    buffer >> serializable;
    return protocol_sm;
}
