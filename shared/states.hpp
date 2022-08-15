#ifndef STATES_HPP
#define STATES_HPP

#include <array>

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

        private:
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

    class WaitingForResponse : public State {
        public:
            WaitingForResponse(StateMachine* state_machine)
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

    /**
     * @brief State machine containing all protocol states
     *
     */
    class StateMachine {
        public:
            void parse_byte(const char t_byte);
            void reset();

        private:
            // Common protocol variables for all states
            constexpr static size_t PROTOCOL_BUFFER_SIZE = 1024;
            std::array<uint8_t, PROTOCOL_BUFFER_SIZE> read_buffer;

            // States
            ReadingHeader s_reading_header = ReadingHeader(this);
            WaitingForResponse s_waiting_for_response = WaitingForResponse(this);
            ReadingBody s_reading_body = ReadingBody(this);
            ReadingTrailing s_reading_trailing = ReadingTrailing(this);
            
            State* const INIT_STATE = &s_reading_header;
            State* m_current_state = INIT_STATE;

            friend ReadingHeader;
            friend WaitingForResponse;
            friend ReadingBody;
            friend ReadingTrailing;
    };
};

#endif // STATES_HPP
