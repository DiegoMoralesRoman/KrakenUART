#include "../../headers/protocol_states.hpp"

___impl::ProtocolSMContext::ProtocolSMContext(ProtocolSM* sm) :
    s_idle(sm),
    s_reading_header(sm)
{}
