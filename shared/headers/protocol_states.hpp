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
     * Copy on https://mermaid.live/edit

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

        private:
            size_t bytes_read = 0;
    };

    class ReadingBody : public ProtocolState {
        public:
            ReadingBody(ProtocolSM* sm) : ProtocolState{sm} {}

            ProtocolState* on_enter() override;
            void on_exit() override;
            ProtocolState* signal(const Signal_t signal) override;
    };


    // ==================================================
    // End of state definitions
    // ==================================================

    // State machine context
    namespace ___impl {
        struct ProtocolSMContext;

        /**
         * @brief Protocol state machine context
         */
        struct ProtocolSMContext {
            ProtocolSMContext(ProtocolSM* sm);

            // Global state variables
            char* current_buffer = nullptr;
            size_t ammount_read = 0;
        

            // States
            Idle s_idle;
            ReadingHeader s_reading_header;
            ReadingBody s_reading_body;
            
            // Other variables
            ProtocolSM* sm;
        };
    }
}

#endif // PROTOCOL_STATES_HPP
