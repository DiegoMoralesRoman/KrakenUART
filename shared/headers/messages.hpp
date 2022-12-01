#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "utils.hpp"
#include "primitives.hpp"

namespace protocol::messages {
    /**
     * @brief This enum contains all the available protocol messages implemented. 
     */
    enum Types : uint8_t {
        // Protocol messages
        TEST
        // RMI messages

        // Custom messages
    };

    // ==================================================
    // Protocol messages
    // ==================================================
    
    /**
     * @brief Protocol message base class containing all methods neccessary to send a message
     * @details
     * Sending this kind of message allows the protocol to identify the message types. This is done by adding a header containing
     * message type information that is processed automatically.
     */
    class ProtocolMessage : public serial::Serializable {
        public:
            /**
             * @brief This method provides the type value of the class. Return an enum element of type Type.
             * @details 
             * Every class has to have a message type that allows identification in the other end of the communication.
             */
            virtual uint8_t type() const = 0;
    };

    /**
     * @brief Header of every message it contains the length of the message and a checksum of both the header and the entire message
     * @details The checksum calculation is performed in the following order:
     */
    class Header : public serial::Serializable {
        public:
            char *const serialize(char *const buffer) const override;
            char *const deserialize(char *const buffer) override;
        private:
            // TODO: add size and checksum variables
    };


    // ==================================================
    // Protocol messages
    // ==================================================
    class Test : public ProtocolMessage {
        public:
            primitives::Int32 number;

            virtual size_t size() const override {return number.size();}

            char *const serialize(char *const buffer) const override;
            char *const deserialize(char *const buffer) override;

            uint8_t type() const override {return TEST;}
    };

    // ==================================================
    // RMI messages
    // ==================================================
    // #include "rmi.hpp"

    // ==================================================
    // Custom messages
    // ==================================================
}

#endif // MESSAGES_HPP
