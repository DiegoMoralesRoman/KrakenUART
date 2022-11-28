#ifndef PROTOCOL_STATES_HPP
#define PROTOCOL_STATES_HPP

#include "serial.hpp"
#include "states.hpp"
#include <unistd.h>

namespace protocol::states {

    namespace ___impl {struct ProtocolSMContext;}

    namespace signals {
        enum {
            BYTE_RCV,
            SEND,
            HASH_ERROR
        };
    }

    /**
    * State machine
    */
    using ProtocolSM = StateMachine<___impl::ProtocolSMContext>;

    template class State<___impl::ProtocolSMContext>;
    using ProtocolState = State<___impl::ProtocolSMContext>;
    // ==================================================
    // States definitions
    // ==================================================

    /**
     * State machine diagram
     *
     graph LR
        Idle --"on_byte_rcv"--> RH[OnReadingHeader]
        RH --"on_byte"--> RH
        RH --"read_header"--> RB[ReadingBody]
        RB --"on_byte"--> RB
        RH --"on_hash_error"--> Flush
        RB --"on_stream_error"--> Flush
        RB --"on_msg_ok"--> IM[InterpretMessage]
        IM --"on_protocol_message"-->Act --> Flush
        IM --"on_data_message"--> NotifyUser --> Flush
        Flush --> Idle
        Idle --"on_sending"--> WritingMessage
     */

    /**
     * @brief Base state where sending and receiving are possibilities
     */
    class Idle : public ProtocolState {
        public:
            Idle(ProtocolSM* sm) : ProtocolState{sm} {}

            State* on_enter() override;
            void on_exit() override;
            ProtocolState* signal(const Signal_t signal) override;
    };

    /**
     * @brief Reading the header of the message
     * @details Before leaving, after reading the header, the checksum is calculated
     */
    class ReadingHeader : public ProtocolState {
        public:
            ReadingHeader(ProtocolSM* sm) : ProtocolState{sm} {}

            State* on_enter() override;
            void on_exit() override;
            ProtocolState* signal(const Signal_t signal) override;
    };

    // State machine context
    namespace ___impl {
        struct ProtocolSMContext;

        class ProtocolStream : public serial::Stream {
            public:
                ProtocolStream(ProtocolSMContext* ctx)
                    : ctx(ctx) {}

                void write(const char *buff, size_t ammount) override;
                size_t read(char *buff, const size_t ammount) override;
                void flush() override;

            private:
                ProtocolSMContext* ctx;
        };

        struct ProtocolSMContext {
            ProtocolSMContext(ProtocolSM* sm);

            // Global state variables
            char last_rcv_byte = 0;

            // States
            Idle s_idle;
            ReadingHeader s_reading_header;
            
            // Other variables
            ProtocolStream stream = ProtocolStream(this);
            ProtocolSM* sm;
        };
    }
}

#endif // PROTOCOL_STATES_HPP
