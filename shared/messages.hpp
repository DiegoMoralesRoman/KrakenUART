#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include "primitives.hpp"

namespace protocol::messages {
    /**
     * @brief Header of every message it contains the length of the message and a checksum of both the header and the entire message
     */
    class Header : public primitives::Serializable {
        public:
            primitives::Int16 len;
            primitives::Int32 header_checksum;
            primitives::Int32 message_checksum;

            virtual size_t size() const {return len.size() + header_checksum.size() + message_checksum.size();}
            virtual void serialize(char* buffer) const;
            virtual void deserialize(const char* buffer);
        private:
    };
}

#endif // MESSAGES_HPP
