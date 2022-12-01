#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <stdint.h>
#include <stddef.h>

namespace protocol::serial {
    class Serializable;
    
    /**
     * @brief Base class for Serializable objects to dump their data
     */
    class Stream {
        public:
            struct PropagationContext;
            /**
             * @brief Propagates the TODO: finish documentation
             */
            virtual void propagate(const PropagationContext&& ctx) = 0;

            void connect(Stream* other);

            /**
             * @brief Flushes the stream and sends or reads everything
             */
            virtual void flush() = 0;


            struct PropagationContext {
                size_t ammount;
            };

        private:
            Stream* m_connected_stream;
    };

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
        ___impl::SerializationWrapper& operator<<(___impl::SerializationWrapper& wrapper, const serial::Serializable& serializable);
        // Deserialization operators
        ___impl::SerializationWrapper operator>>(char *const buffer, serial::Serializable& serializable);
        ___impl::SerializationWrapper& operator>>(___impl::SerializationWrapper& wrapper, serial::Serializable& serializable);
}

#endif // SERIAL_HPP
