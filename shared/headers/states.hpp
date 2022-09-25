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

        private:
            bool ack_read_pending = false;
            bool sent_nack = false;
    };


    class ReadingBody : public State {
        public:
            // Initialization
            void init(size_t len, decltype(messages::Header::message_type)::Underlying_t type);

            ReadingBody(StateMachine* state_machine)
                : State{state_machine} {}

            virtual State* parse_byte(const char t_byte);
            virtual void on_enter();
            virtual void on_exit();

        private:
            size_t m_body_len = 0;
            decltype(messages::Header::message_type)::Underlying_t m_message_type;

            bool ack_read_pending = false;
            bool sent_nack = false;
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

    class IgnoringN : public State {
        public:
            IgnoringN(StateMachine* state_machine)
                : State{state_machine} {}

            virtual State* parse_byte(const char t_byte);
            virtual void on_enter();
            virtual void on_exit();

            void ignore(size_t ammount, State* return_state);

        private:
            size_t to_ignore = 0;
            State* return_state;
    };

    /**
     * @brief State machine containing all protocol states
     *
     */
    class StateMachine {
        public:
            StateMachine() {m_current_state->on_enter();}
            /**
             * @brief Parses a byte accounting for the current state of the state machine
             * @param t_byte Byte to parse
             */
            void parse_byte(const char t_byte);
            /**
             * @brief Resets the state machine (shouldn't be neccessary)
             */
            void reset();

            /**
             * @brief Starts to send a message to the connected device
             * @param message Message to send
             * @details This operation will be immediate so if the state machine is currently sending a message it will be corrpted (use with caution).
             */
            void send_message(const primitives::Serializable& serializable);
            /**
             * @brief Starts to send a message to the connected device
             * @param message Message to send (protocol message, the header will be added automatically)
             * @details This operation will be immediate so if the state machine is currently sending a message it will be corrpted (use with caution).
             */
            void send_message(const messages::ProtocolMessage& message);

            // Callbacks and utilities
            /**
             * @brief Function to use to send a buffer to the connected device
             */
            functor<void(const char*, const size_t len)> m_send_buffer;
            /**
             * @brief Callback to call when a message has been received
             * @param char* message received
             * @param Int8::Underlying_t Protocol message type (neccessary to properly deserialize)
             */
            functor<void(const char* buffer, primitives::Int8::Underlying_t type)> m_msg_received = [](const char*, primitives::Int8::Underlying_t) {};
            /**
             * @brief Callback to call when the message has been sent and confirmed by the conencted device
             */
            functor<void()> sent_complete_handler = [](){};
        protected:
            // Common protocol variables for all states
            /**
             * @brief Internal buffer size
             * @details This has to accomodate two of the largest messages that can be sent or received
             */
            constexpr static size_t PROTOCOL_BUFFER_SIZE = 1024;
            /**
             * @brief Buffer contaning the currently sent message and the read bytes
             * @details It has the following structure:
             *      [#####>-----#####]
             *        ^          ^
             *       read buffer |
             *                  write buffer
             * The read buffer will grow when new data is received and the write buffer will not change until a new message is prepared
             */
            std::array<char, PROTOCOL_BUFFER_SIZE> m_msg_buffer;
            // Buffer tracking variables
            /**
             * @brief Bytes read 
             */
            size_t m_ammount_read = 0;
            /**
             * @brief Position of the start of the message to send 
             * @details
             *      [#####>-----#####]
             *                  ^
             *                 Points to this position in the internal buffer
             */
            char* m_write_start = 0;

            // States
            ReadingHeader s_reading_header = ReadingHeader(this);
            WaitingHeaderACK s_waiting_header_ack = WaitingHeaderACK(this);
            WaitingBodyACK s_waiting_body_ack = WaitingBodyACK(this);
            ReadingBody s_reading_body = ReadingBody(this);
            ReadingTrailing s_reading_trailing = ReadingTrailing(this);
            IgnoringN s_ignoring_n = IgnoringN(this);

            // Utility methods
            /**
             * @brief Adds a byte to the read portion of the buffer (incrementing the count)
             * @param t_byte Byte to add to the buffer
             */
            void push_to_buffer(uint8_t t_byte);
            /**
             * @brief Resets the read portion of the buffer
             */
            void reset_read();
            /**
             * @brief Resets the write portion of the buffer
             */
            void reset_write();

            /**
             * @brief Sets the write buffer position and deserializes the data into the buffer
             * @param msg Serializable object to send
             */
            void set_current_message(const primitives::Serializable& msg);
            /**
             * @brief Sets the write buffer position and deserializes the data into the buffer
             * @param Protocol message to send (the header is added automatically)
             */
            void set_current_message(const messages::ProtocolMessage& msg);

            /**
             * @brief Sends an ACK to the connected device and a boolean that says if there is more data after the ack
             */
            void send_ack(uint32_t ack, bool last);

            /**
             * @brief Sends the header stored in the write portion of the buffer
             */
            void send_header_now();
            /**
             * @brief Sends the body stored in the write portion of the buffer
             */
            void send_body_now();

            /**
             * @brief Flag that is true when the last state has finished the message sending operation
             */
            bool m_finished_sending = false;
            /**
             * @brief Flag that is true when this state machine has priority to sending a message in case of collision
             * TODO: finish collision avoidance implementation
             */
            
        public: bool m_has_send_priority = false; protected:

            /**
             * @brief Indicates if the header of a message is being sent
             */
            bool comm_started = false;
        private:
            /**
             * @brief Initial state (set on construction and reset)
             */
            State* const INIT_STATE = &s_reading_header;
            /**
             * @brief Current state
             */
            State* m_current_state = INIT_STATE;
            /**
             * @brief Previous state
             */
            State* m_prev_state = INIT_STATE;

            friend ReadingHeader;
            friend WaitingHeaderACK;
            friend WaitingBodyACK;
            friend ReadingBody;
            friend ReadingTrailing;
            friend IgnoringN;
    };
}

#endif // STATES_HPP
