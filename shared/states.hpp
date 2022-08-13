#ifndef STATES_HPP
#define STATES_HPP

#include <array>

namespace protocol::states {
    /**
     * @brief Base class for all of the protocol states
     * @details All protocol states must derive from this class
     */
    class State {
        public:
            /**
             * @brief Sends a byte to the current state of the protocol
             * 
             * @param t_byte Byte to process
             */
            virtual void parse_byte(const char t_byte) = 0;
            virtual void on_enter() = 0;
            virtual void on_exit() = 0;

        protected:
            static constexpr size_t READ_BUFFER_SIZE = 0;
            static std::array<char, READ_BUFFER_SIZE> m_read_buffer;
    };

    // ==================================================
    // Common protocol methods
    // ==================================================
    


    // ==================================================
    // States definitions
    // ==================================================
    class ReadingHeader : public State {
        public:
            virtual void parse_byte(const char t_byte);
            virtual void on_enter();
            virtual void on_exit();
    };

    class ReadingBody : public State {
        public:
            virtual void parse_byte(const char t_byte);
            virtual void on_enter();
            virtual void on_exit();
    };
};

#endif // STATES_HPP
