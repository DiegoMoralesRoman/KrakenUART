#ifndef BASE_128_HPP
#define BASE_128_HPP

#include <stdint.h>
#include <stddef.h>

namespace protocol::base128 {
    class Serializable;

    class Stream {
        public:
            virtual void read(char* buff, const size_t ammount) = 0;
            virtual void write(const char* buff, size_t ammount) = 0;
            
            virtual void flush() = 0;
    };

    // Base128 encoding and decoding
    namespace ___impl {
        class EncoderContinuation {
            public:
                EncoderContinuation(Stream* stream) : stream(stream) {}

                EncoderContinuation& operator<<(const Serializable& serializable);
                EncoderContinuation& operator>>(Serializable& serializable);
            private:
                Stream* stream;
                uint8_t tmp_buffer[8] = {0};
        };
    }

    ___impl::EncoderContinuation operator<<(Stream& stream, const Serializable& serializable);
    ___impl::EncoderContinuation operator>>(Stream& stream, Serializable& serializable);

    /**
     *  @brief Base serialization class
     *
     */
    class Serializable {
        public:
            virtual void serialize(Stream& stream) const = 0;
            virtual size_t size() const = 0;
            virtual void deserialize(Stream& stream) = 0;
    };
}

#endif // BASE_128_HPP
