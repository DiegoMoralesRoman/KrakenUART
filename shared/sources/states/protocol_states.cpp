#include "../../headers/protocol_states.hpp"

___impl::ProtocolSMContext::ProtocolSMContext(ProtocolSM* sm) :
    s_idle(sm),
    s_reading_header(sm),
    s_reading_body(sm),
    sm(sm)
{}

// Protocol stream
size_t ___impl::ProtocolStream::read(char *buff, const size_t ammount) {

    return ammount;
}

void ___impl::ProtocolStream::write(const char *buff, size_t ammount) {
    for (size_t i = 0; i < ammount; i++) {
        char c = buff[i];
        ctx->last_rcv_byte = c;
        ctx->sm->signal(signals::BYTE_RCV);
    }
}

void ___impl::ProtocolStream::flush() {

}
