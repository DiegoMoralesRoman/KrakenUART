#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "utils.hpp"
#include "primitives.hpp"

namespace protocol::messages {
    // Message types
    enum Types : uint8_t {
        // Protocol messages
        ADMIN,
        // RMI messages
        RMICALL,
        RMIRETURN

        // Custom messages
    };

    // ==================================================
    // Protocol messages
    // ==================================================
    
    /**
     * @brief Protocol message base class containing all methods neccessary to send a message
     */
    class ProtocolMessage : public base128::Serializable {
        public:
            virtual uint8_t type() const = 0;
    };

    /**
     * @brief Header of every message it contains the length of the message and a checksum of both the header and the entire message
     * @details The checksum calculation is performed in the following order:
     */
    class Header : public base128::Serializable {
        public:
            virtual void serialize(base128::Stream& stream) const;
            virtual void deserialize(base128::Stream& stream);
        private:
    };


    /**
     * @brief 
     */
    class Admin : public ProtocolMessage {
        public:
            primitives::Int8 ack;

            virtual size_t size() const override {return ack.size();}

            virtual void serialize(base128::Stream& stream) const override;
            virtual void deserialize(base128::Stream& stream) override;

            virtual uint8_t type() const override {return ADMIN;}
    };

    // ==================================================
    // RMI messages
    // ==================================================


    // ==================================================
    // Custom messages
    // ==================================================
}

#endif // MESSAGES_HPP
