#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <string>

namespace protocol::primitives {
    /**
     *  @brief Base serialization class
     *
     */
    class Serializable {
        public:
            /**
             * @brief Adds the data of the structure to the buffer starting at the position 0
             * 
             * @param buffer Buffer to store the serialized data
             * @return passed buffer advanced to the next serializable position
             */
            virtual char* serialize(char* buffer) const = 0;
            /**
             * @brief Returns the size of the serialized structure
             *
             * @return Size of the serialized structure
             */
            virtual size_t size() const = 0;
            /*
             * @brief Reads data from a buffer and stores it inside the structure
             * 
             * @param buffer Buffer where the data is stored
             * @return passed buffer advanced to the next serializable position
             */
            virtual const char* deserialize(const char* buffer) = 0;
    };

    // Operators
    namespace ___impl {
        template<typename Buff_t>
        struct SerializableContinuation {
            operator Buff_t() {return buffer + cum_offset;}
            SerializableContinuation<char*>&& operator<<(const Serializable& ser);
            SerializableContinuation<const char*>&& operator>>(Serializable& ser);

            Buff_t buffer;
            size_t cum_offset = 0;
        };
    }

    ___impl::SerializableContinuation<char*> operator<<(char* buffer, const Serializable& ser);
    ___impl::SerializableContinuation<const char*> operator>>(const char* buffer, Serializable& ser);

    // ==================================================
    // Primitive protocol types
    // ==================================================

        // --- Integer types ---

    class Int32 : public Serializable {
        public:
            // Utility constructors
            Int32(uint32_t n) : m_value(n) {}
            Int32() = default;
            operator uint32_t() const {return m_value;}
            operator size_t() const {return static_cast<size_t>(m_value);}
            Int32& operator=(const uint32_t value) {m_value = value; return *this;}

            // Virtual methods overloading
            static constexpr size_t static_size() {return 4;}
            virtual size_t size() const {return static_size();}
            virtual char* serialize(char* buffer) const;
            virtual const char* deserialize(const char* buffer);
        private:
            uint32_t m_value = 0;
    };
    using Size = Int32;

    class Int16 : public Serializable {
        public:
            Int16(uint16_t n) : m_value(n) {}
            Int16() = default;
            operator uint16_t() const {return m_value;}
            Int16& operator=(const uint16_t value) {m_value = value; return *this;}
            // Virtual methods overloading

            static constexpr size_t static_size() {return 2;}
            virtual size_t size() const {return static_size();}
            virtual char* serialize(char* buffer) const;
            virtual const char* deserialize(const char* buffer);
        private:
            uint16_t m_value = 0;
    };

    class Int8 : public Serializable {
        public:
            Int8(uint8_t n) : m_value(n) {}
            Int8() = default;
            operator uint8_t() const {return m_value;}
            Int8& operator=(const uint8_t value) {m_value = value; return *this;}
            // Virtual methods overloading

            static constexpr size_t static_size() {return 1;}
            virtual size_t size() const {return static_size();}
            virtual char* serialize(char* buffer) const;
            virtual const char* deserialize(const char* buffer);
        private:
            uint8_t m_value = 0;
    };
        
        // --- String types
    class String : public Serializable {
        public:
            String(const char* str);
            String() = default;

            std::string string;
            // Virtual methods overloading
            virtual size_t size() const {return string.length() + Int32::static_size();}
            virtual char* serialize(char* buffer) const;
            virtual const char* deserialize(const char* buffer);
    };
}

#endif // PRIMITIVES_HPP
