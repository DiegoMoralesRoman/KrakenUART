#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "utils.hpp"
#include "primitives.hpp"

namespace protocol::messages {
    // Message types
    enum Types : uint8_t {
        // Protocol messages
        HEADER,
        ADMIN,
        // RMI messages
        RMICALL,
        RMIRETURN

        // Custom messages
    };

    // ACK
    enum ACKTypes : uint32_t {
        HEADER_ACK = uahruart::utils::hash_uint32(0),
        HEADER_NACK = uahruart::utils::hash_uint32(1),
        BODY_ACK = uahruart::utils::hash_uint32(2),
        BODY_NACK = uahruart::utils::hash_uint32(3),
        UNDEF_ACK = uahruart::utils::hash_uint32(4),
        CORR_ACK = uahruart::utils::hash_uint32(5),
        FIRST_MSG = uahruart::utils::hash_uint32(6)
    };


    // ==================================================
    // Protocol messages
    // ==================================================
    
    /**
     * @brief Protocol message base class containing all methods neccessary to send a message
     */
    class ProtocolMessage : public primitives::Serializable {
        public:
            virtual uint8_t type() const = 0;
    };

    /**
     * @brief Header of every message it contains the length of the message and a checksum of both the header and the entire message
     * @details The checksum calculation is performed in the following order:
     */
    class Header : public primitives::Serializable {
        public:
            // Message values
            /** @brief length of the message body */
            mutable primitives::Int16 len;
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

            virtual size_t size() const {return Header::static_size();}
            static constexpr size_t static_size() {return decltype(len)::static_size() + decltype(header_checksum)::static_size() + decltype(message_type)::static_size();}
            /**
             * @details The header checksum is generated at serialization time
             */
            virtual char* serialize(char* buffer) const;
            virtual const char* deserialize(const char* buffer);
        private:
            /**
             * @brief Calculates the current checksum of the header
             */
            uint32_t get_checksum() const;
            /**
             * After deserialization this variable will tell if the header is valid (not corrupted).
             */
            bool m_is_valid_checksum = false;
    };


    /**
     * @brief 
     */
    class Admin : public ProtocolMessage {
        public:
            primitives::Int8 ack;

            virtual size_t size() const override {return ack.size();}
            virtual char* serialize(char* buffer) const override;
            virtual const char* deserialize(const char* buffer) override;

            virtual uint8_t type() const override {return ADMIN;}
    };

    // ==================================================
    // RMI messages
    // ==================================================
    using RMIArg = primitives::Int16;
    class RMICall : public ProtocolMessage {
        public:
            primitives::Int32 UUID;
            primitives::Int32 call_hash;
            RMIArg arg;

            virtual size_t size() const override {return UUID.size() + call_hash.size() + arg.size();}
            virtual char* serialize(char* buffer) const override;
            virtual const char* deserialize(const char* buffer) override;

            virtual uint8_t type() const override {return RMICALL;}
    };

    class RMIReturn : public ProtocolMessage {
        public:
            primitives::Int32 UUID;
            RMIArg ret_value;
            primitives::Bool ok;

            virtual size_t size() const override {return UUID.size() + ret_value.size() + ok.size();}
            virtual char* serialize(char* buffer) const override;
            virtual const char* deserialize(const char* buffer) override;

            virtual uint8_t type() const override {return RMIRETURN;}
    };


    // ==================================================
    // Custom messages
    // ==================================================
}

#endif // MESSAGES_HPP
