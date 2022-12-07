#include "messages.hpp"
#include "primitives.hpp"
#include "states.hpp"
#include "utils.hpp"
#include "serial.hpp"
#include "protocol_states.hpp"

namespace uahruart::protocol {
    class ProtocolBuffer {
        public:
            /**
             * @brief Checks buffer to see if the hash coincides with what's in the buffer
             */
            virtual bool check_hash(utils::hash_t hash) = 0;
            
            /**
             * @brief Serialization
             * @return Has to return the internal buffer to continue serialization
             */
            virtual char* operator<<(const protocol::serial::Serializable& serializable) = 0;

            /**
             * @brief Deserialization
             * @return Has to return the internal buffer to continue deserialization
             */
            virtual const char* operator>>(protocol::serial::Serializable& serializable) = 0;
    };
}
