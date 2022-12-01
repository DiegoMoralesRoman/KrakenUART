#include "../../headers/protocol_states.hpp"

___impl::ProtocolSMContext::ProtocolSMContext(ProtocolSM* sm) :
    s_idle(sm),
    s_reading_header(sm),
    s_reading_body(sm),
    sm(sm)
{}

// Protocol stream
void ___impl::ProtocolStream::propagate(const PropagationContext &&ctx) {

}

void ___impl::ProtocolStream::flush() {

}
