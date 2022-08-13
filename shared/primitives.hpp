#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include <stddef.h>
#include <stdint.h>

namespace protocol::primitives {
    /**
     *  @brief Base serialization class
     *
     */
    class Serializable {
        public:
            virtual void serialize(char* buffer) const = 0;
            virtual size_t size() const = 0;
            virtual void deserialize(const char* buffer) = 0;
    };

    // Operators
    namespace ___impl {
        struct SerializableContinuation {
            operator char*() {return buffer;}
            SerializableContinuation&& operator<<(const Serializable& ser);
            SerializableContinuation&& operator>>(Serializable& ser);

            char* buffer;
            size_t cum_offset = 0;
        };
    }

    ___impl::SerializableContinuation operator<<(char* buffer, const Serializable& ser);
    ___impl::SerializableContinuation operator>>(char* buffer, Serializable& ser);

    // ==================================================
    // Primitive protocol types
    // ==================================================

        // --- Integer types ---

    class Int32 : public Serializable {
        public:
            // Utility constructors
            Int32(uint32_t n) : m_value(n) {}
            Int32() = default;
            operator uint32_t() {return m_value;}
            // Virtual methods overloading
            virtual size_t size() const {return 4;}
            virtual void serialize(char* buffer) const;
            virtual void deserialize(const char* buffer);
        private:
            uint32_t m_value = 0;
    };

    class Int16 : public Serializable {
        public:
            Int16(uint16_t n) : m_value(n) {}
            Int16() = default;
            operator uint16_t() {return m_value;}
            // Virtual methods overloading
            virtual size_t size() const {return 2;}
            virtual void serialize(char* buffer) const;
            virtual void deserialize(const char* buffer);
        private:
            uint16_t m_value = 0;
    };

    class Int8 : public Serializable {
        public:
            Int8(uint8_t n) : m_value(n) {}
            Int8() = default;
            operator uint8_t() {return m_value;}
            // Virtual methods overloading
            virtual size_t size() const {return 1;}
            virtual void serialize(char* buffer) const;
            virtual void deserialize(const char* buffer);
        private:
            uint8_t m_value = 0;
    };
        
        // --- String types
    class String : public Serializable {
        public:

        private:

    };
}

#endif // PRIMITIVES_HPP
