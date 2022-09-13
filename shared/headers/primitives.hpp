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
        /**
         * @brief Intermediate type that stores the current serialization step state
         * @details This data type stores the current offset of the serialization to continue the next one. 
         * It can also be converted into the original type implicitly. This makes this type completely transparent to the user
         * With this operator the following syntax is made available for the user
         * buff << ser1 << ser2 << ... << sern;
         * buff >> ser1 >> ser2 >> ... >> sern;
         * This will serialize and then deserialize multiple objects in the same order
         */
        template<typename Buff_t>
        struct SerializableContinuation {
            /**
             * @brief Conversion to the base type (Buff_t)
             *
             * @return Buffer in the next available position for serialization
             */
            operator Buff_t() {return buffer + cum_offset;}
            /**
             * @brief Serialization operator
             * @param ser Serializable object to continue the serialization
             * @return Intermediate state ready for the next serialization
             */
            SerializableContinuation<char*> operator<<(const Serializable& ser);
            /**
             * @brief Deserialization operator
             * @param ser Serializable object to continue the deserialization
             * @return Intermediate state ready for the next serialization
             */
            SerializableContinuation<const char*> operator>>(Serializable& ser);

            /**
             * @brief Absolute buffer where the date is stored/obtained
             */
            Buff_t buffer;
            /**
             * @brief Current serialization/deserialization offset relative to the base buffer
             */
            size_t cum_offset = 0;
        };
    }

    /**
     * @brief Used to serialize an object extending the Serializable class
     * @details The return type allows to concatenate this operators to serialize multiple objects in the same expression
     *      buff << ser1 << ser2 << ... << sern;
     *
     * @param buffer Buffer to store the objects into
     * @param ser Serializabl object that has to be stored in the buffer
     * @return Intermediate type that allows the concatenation of multiple serialization operations
     */
    ___impl::SerializableContinuation<char*> operator<<(char* buffer, const Serializable& ser);
    /**
     * @brief Used to deserialize an object extending the Serializable class
     * @details The return type allows to concatenate this operators to deserialize multiple objects in the same expression
     *      buff >> ser1 >> ser2 >> ... >> sern;
     *
     * @param buffer Buffer from where to extract the data for the deserialization
     * @param ser Serializable object to store the data into
     */
    ___impl::SerializableContinuation<const char*> operator>>(const char* buffer, Serializable& ser);

    // ==================================================
    // Primitive protocol types
    // ==================================================

        // --- Integer types ---

    class Int32 : public Serializable {
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
            virtual size_t size() const {return static_size();}
            virtual char* serialize(char* buffer) const;
            virtual const char* deserialize(const char* buffer);
        private:
            uint32_t m_value = 0;
    };
    using Size = Int32;
    using ACK = Int32;
    using Checksum = Int32;

    class Int16 : public Serializable {
        public:
            using Underlying_t = uint16_t;
            // Utility constructors
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
            using Underlying_t = uint8_t;
            // Utility constructors
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
            using Underlying_t = std::string;
            // Utility constructors
            String(const char* str);
            String() = default;

            std::string string;
            // Virtual methods overloading
            virtual size_t size() const {return string.length() + Int32::static_size();}
            virtual char* serialize(char* buffer) const;
            virtual const char* deserialize(const char* buffer);
    };

    // Boolean constants
    using Bool = Int8;
    extern const Int8 TRUE;
    extern const Int8 FALSE;
}

#endif // PRIMITIVES_HPP
