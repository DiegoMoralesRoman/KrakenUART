#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <string>

#include "serial.hpp"

namespace uahruart::protocol::primitives {
    // ==================================================
    // Primitive protocol types
    // ==================================================

        // --- Integer types ---

    class Int32 : public serial::Serializable {
        public:
            using Underlying_t = uint32_t;
            // Utility constructors
            Int32(uint32_t n) : m_value(n) {}
            Int32() = default;
            operator uint32_t() const {return m_value;}
            //operator size_t() const {return static_cast<size_t>(m_value);}
            Int32& operator=(const uint32_t value) {m_value = value; return *this;}

            // Virtual methods overloading
            static constexpr size_t static_size() {return 4;}
            virtual size_t size() const override {return static_size();}
            virtual const uahruart::protocol::id::IDs type_id() const override {return uahruart::protocol::id::IDs::PRIMITIVE_I32;}

            char *const serialize(char *const buffer) const override;
            const char *const deserialize(const char *const buffer) override;
        private:
            uint32_t m_value = 0;
    };
    using Size = Int32;
    using ACK = Int32;
    using Checksum = Int32;

    class Int16 : public serial::Serializable {
        public:
            using Underlying_t = uint16_t;
            // Utility constructors
            Int16(uint16_t n) : m_value(n) {}
            Int16() = default;
            operator uint16_t() const {return m_value;}
            Int16& operator=(const uint16_t value) {m_value = value; return *this;}
            // Virtual methods overloading

            static constexpr size_t static_size() {return 2;}
            virtual size_t size() const override {return static_size();}
            virtual const uahruart::protocol::id::IDs type_id() const override {return uahruart::protocol::id::IDs::PRIMITIVE_I16;}

            char *const serialize(char *const buffer) const override;
            const char *const deserialize(const char *const buffer) override;
        private:
            uint16_t m_value = 0;
    };

    class Int8 : public serial::Serializable {
        public:
            using Underlying_t = uint8_t;
            // Utility constructors
            Int8(uint8_t n) : m_value(n) {}
            Int8() = default;
            operator uint8_t() const {return m_value;}
            Int8& operator=(const uint8_t value) {m_value = value; return *this;}
            // Virtual methods overloading

            static constexpr size_t static_size() {return 1;}
            virtual size_t size() const override {return static_size();}
            virtual const uahruart::protocol::id::IDs type_id() const override {return uahruart::protocol::id::IDs::PRIMITIVE_I8;}

            char *const serialize(char *const buffer) const override;
            const char *const deserialize(const char *const buffer) override;
        private:
            uint8_t m_value = 0;
    };
        
        // --- String types
    class String : public serial::Serializable {
        public:
            using Underlying_t = std::string;
            // Utility constructors
            String(const char* str);
            String() = default;

            std::string string;
            // Virtual methods overloading
            virtual size_t size() const override {return string.length() + Int32::static_size();}
            virtual const uahruart::protocol::id::IDs type_id() const override {return uahruart::protocol::id::IDs::PRIMITIVE_STR;}

            char *const serialize(char *const buffer) const override;
            const char *const deserialize(const char *const buffer) override;
    };

    // Boolean constants
    using Bool = Int8;
    extern const Int8 TRUE;
    extern const Int8 FALSE;
}

#endif // PRIMITIVES_HPP
