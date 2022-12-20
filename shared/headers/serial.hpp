#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <stdint.h>
#include <stddef.h>

namespace protocol::serial {
    /**
     *  @brief Base serialization class
     *
     */
    class Serializable {
        public:
            /**
             * @brief 
             */
            virtual char *const serialize(char *const buffer) const = 0;
            /**
             * @brief 
             */
            virtual size_t size() const = 0;
            /**
             * @brief 
             */
            virtual char *const deserialize(char *const buffer) = 0;
    };


    // ==================================================
    // Primitive protocol types
    // ==================================================
    namespace ___impl {
        struct SerializationWrapper {
            char *const buffer;
            size_t base_position;

            operator char*();
        };
    };

    // Serialization operatos
    ___impl::SerializationWrapper operator<<(char *const buffer, const serial::Serializable& serializable);
    ___impl::SerializationWrapper& operator<<(___impl::SerializationWrapper&& wrapper, const serial::Serializable& serializable);
    // Deserialization operators
    ___impl::SerializationWrapper operator>>(char *const buffer, serial::Serializable& serializable);
    ___impl::SerializationWrapper& operator>>(___impl::SerializationWrapper&& wrapper, serial::Serializable& serializable);
}

#endif // SERIAL_HPP
