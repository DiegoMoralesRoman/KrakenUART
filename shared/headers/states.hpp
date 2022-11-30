#ifndef STATES_HPP
#define STATES_HPP

#include <array>
#include "utils.hpp"
#include "primitives.hpp"
#include "messages.hpp"
#include <stddef.h>

namespace protocol::states {
    template<typename Context>
    class StateMachine;

    using Signal_t = unsigned char;

    /**
     * @brief Base class for all of the protocol states
     * @details All protocol states must derive from this class
     */
    template<typename Context>
    class State {
        public:
            State(StateMachine<Context>* state_machine)
                : m_state_machine(state_machine) {}
            State() = delete;
                
            /**
             * @brief Signals the state of an event
             * @param signal signal that has been sent
             * @return State* to the next state
             */
            virtual State* signal(const Signal_t signal) = 0;
            /**
             * @brief Runs every time the state machine enters the state
             */
            virtual State* on_enter() = 0;
            /**
             * @brief Runs every time the state machine exits the state
             */
            virtual void on_exit() = 0;

        protected:
            StateMachine<Context>* m_state_machine;
    };


    /**
     * @brief State machine containing all protocol states
     *
     */
    template<typename Context>
    class StateMachine {
        public:
            /**
             * @brief State machine destructor
             * @details Calls on_exit() method of current state
             */
            ~StateMachine();

            /**
             * @details Sends an update signal to the current state
             * @param signal Signal to send
             */
            void signal(const Signal_t signal);

            /**
             * @brief Sets the current state of the state machine
             * @details Calls on_enter() method of the state
             * @param state State to change to
             */
            void set_state(State<Context>* state);
            
            /**
             * @brief Context available to the states
             * @details This context conatins "global" variables
             */
            Context ctx = Context(this);
        private:
            State<Context>* m_current_state = nullptr, *m_prev_state = nullptr;
    };
}

#include "../sources/states.tcc"

#endif // STATES_HPP

