#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <stdint.h>
#include <stddef.h>

#include "msg_ids.hpp"

namespace uahruart::protocol::serial {
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
            virtual const char *const deserialize(const char *const buffer) = 0;

            /**
             * @brief 
             */
            virtual const uahruart::protocol::id::IDs type_id() const = 0;
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
    ___impl::SerializationWrapper operator>>(const char *const buffer, serial::Serializable& serializable);
    ___impl::SerializationWrapper& operator>>(___impl::SerializationWrapper&& wrapper, serial::Serializable& serializable);
}

#endif // SERIAL_HPP
