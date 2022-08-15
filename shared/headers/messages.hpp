#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "primitives.hpp"

namespace protocol::messages {
    // Message types
    enum Types : uint8_t {
        // Protocol messages
        HEADER,
        ADMIN

        // Custom messages
    };

    // ==================================================
    // Protocol messages
    // ==================================================

    /**
     * @brief Header of every message it contains the length of the message and a checksum of both the header and the entire message
     * @details The checksum calculation is performed in the following order:
     */
    class Header : public primitives::Serializable {
        public:
            // Message values
            /** @brief length of the message body */
            primitives::Int16 len;
            /** @brief checksum of the header only */
            mutable primitives::Int32 header_checksum;
            /**
              * @brief Message type (defined in protocol::messages::Types)
              * @details Only use this value when it has been deserialized (otherwise use the virtual size() method)
              */
            primitives::Int8 message_type;

            /**
             * @brief Checks if the value of the checksum is valid according to the other variables
             * @return true if the checksum coincides with the values
             */
            bool is_valid() {return m_is_valid_checksum;}

            virtual size_t size() const {return len.size() + header_checksum.size() + message_type.size();}
            /**
             * @details The header checksum is generated at serialization time
             */
            virtual char* serialize(char* buffer) const;
            virtual const char* deserialize(const char* buffer);
            
            /**
             * @brief Returns the type of the message (defined in protocol::messages::Types)
             */
            virtual uint8_t type() const = 0;
        private:
            uint32_t get_checksum() const;

            bool m_is_valid_checksum = false;
    };

    /**
     * @brief 
     */
    class Admin : public Header {
        public:
            primitives::Int8 ack;

            virtual size_t size() const override {return ack.size() + Header::size(); }
            virtual char* serialize(char* buffer) const override;
            virtual const char* deserialize(const char* buffer) override;

            virtual uint8_t type() const override {return Types::ADMIN;}
    };

    // ==================================================
    // RMI messages
    // ==================================================

    // ==================================================
    // Custom messages
    // ==================================================
}

#endif // MESSAGES_HPP
