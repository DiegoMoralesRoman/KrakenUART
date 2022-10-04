#ifndef STATES_HPP
#define STATES_HPP

#include <array>
#include "utils.hpp"
#include "primitives.hpp"
#include "messages.hpp"
#include <stddef.h>

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

    /**
     * @brief State machine containing all protocol states
     *
     */
    class StateMachine {

    };
}

#endif // STATES_HPP
