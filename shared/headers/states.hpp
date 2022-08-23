#ifndef STATES_HPP
#define STATES_HPP

#include <array>
#include "utils.hpp"
#include "primitives.hpp"
#include "messages.hpp"

namespace protocol::states {
    class StateMachine;

    /**
     * @brief Base class for all of the protocol states
     * @details All protocol states must derive from this class
     */
    class State {
        public:
            State(StateMachine* state_machine)
                : m_state_machine(state_machine) {}
            State() = delete;
            /**
             * @brief Sends a byte to the current state of the protocol
             * 
             * @param t_byte Byte to process
             */
            virtual State* parse_byte(const char t_byte) = 0;
            /**
             * @brief Runs every time the state machine enters the state
             */
            virtual void on_enter() = 0;
            /**
             * @brief Runs every time the state machine exits the state
             */
            virtual void on_exit() = 0;

        protected:
            StateMachine* m_state_machine;
    };


    // ==================================================
    // Common protocol methods
    // ==================================================
    


    // ==================================================
    // States definitions
    // ==================================================
    class ReadingHeader : public State {
        public:
            ReadingHeader(StateMachine* state_machine)
                : State{state_machine} {}

            virtual State* parse_byte(const char t_byte);
            virtual void on_enter();
            virtual void on_exit();
    };


    class ReadingBody : public State {
        public:
            // Initialization
            void init(size_t len);

            ReadingBody(StateMachine* state_machine)
                : State{state_machine} {}

            virtual State* parse_byte(const char t_byte);
            virtual void on_enter();
            virtual void on_exit();

        private:
            size_t m_body_len = 0;
    };

    class ReadingTrailing: public State {
        public:
            ReadingTrailing(StateMachine* state_machine)
                : State{state_machine} {}

            virtual State* parse_byte(const char t_byte);
            virtual void on_enter();
            virtual void on_exit();
    };

    class WaitingHeaderACK: public State {
        public:
            WaitingHeaderACK(StateMachine* state_machine)
                : State{state_machine} {}

            virtual State* parse_byte(const char t_byte);
            virtual void on_enter();
            virtual void on_exit();
    };

    class WaitingBodyACK : public State {
        public:
            WaitingBodyACK(StateMachine* state_machine)
                : State{state_machine} {}

            virtual State* parse_byte(const char t_byte);
            virtual void on_enter();
            virtual void on_exit();
    };

    /**
     * @brief State machine containing all protocol states
     *
     */
    class StateMachine {
        public:
            StateMachine() {m_current_state->on_enter();}
            void parse_byte(const char t_byte);
            void reset();

            void send_message(const primitives::Serializable&& serializable);
            void send_message(const messages::ProtocolMessage&& message);

            // Callbacks and utilities
            functor<void(const char*, const size_t len)> m_send_buffer;
        protected:
            // Common protocol variables for all states
            constexpr static size_t PROTOCOL_BUFFER_SIZE = 1024;
            std::array<char, PROTOCOL_BUFFER_SIZE> m_msg_buffer;
            // Buffer tracking variables
            size_t m_ammount_read = 0;
            char* m_write_start = 0;

            // States
            ReadingHeader s_reading_header = ReadingHeader(this);
            WaitingHeaderACK s_waiting_header_ack = WaitingHeaderACK(this);
            WaitingBodyACK s_waiting_body_ack = WaitingBodyACK(this);
            ReadingBody s_reading_body = ReadingBody(this);
            ReadingTrailing s_reading_trailing = ReadingTrailing(this);

            // Utility methods
            void push_to_buffer(uint8_t t_byte);
            void reset_read();
            void reset_write();

            void set_current_message(const primitives::Serializable&& msg);
            void set_current_message(const messages::ProtocolMessage&& msg);
        private:
            State* const INIT_STATE = &s_reading_header;
            State* m_current_state = INIT_STATE;

            friend ReadingHeader;
            friend WaitingHeaderACK;
            friend WaitingBodyACK;
            friend ReadingBody;
            friend ReadingTrailing;
    };
};

#endif // STATES_HPP
