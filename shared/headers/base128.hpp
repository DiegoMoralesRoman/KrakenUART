#ifndef BASE_128_HPP
#define BASE_128_HPP

#include <stdint.h>
#include <stddef.h>

namespace protocol::base128 {
    class Serializable;
    
    /**
     * @brief Base class for Serializable objects to dump their data
     */
    class Stream {
        public:
            /**
             * @brief Reads n bytes and stores them in a buffer. It's recommended to return 0 if no data is available.
             * 
             * @param buff Buffer to store the data
             * @param ammount Ammount of bytes to read
             */
            virtual void read(char* buff, const size_t ammount) = 0;
            /**
             * @brief Writes n bytes obtained from the passed buffer
             * 
             * @param buff Buffer to get data from
             * @param ammount Ammount of bytes to write
             */
            virtual void write(const char* buff, size_t ammount) = 0;
            
            /**
             * @brief Flushes the stream and sends everything
             */
            virtual void flush() = 0;
    };

    // ==================================================
    // Base128 encoding and decoding
    // ==================================================

    namespace ___impl {
        /**
         * @brief This class stores the intermediate state of serialization and deserialization.
         * @details Store the result of the operation stream << serializable or stream >> serialiable to use it later in a bigger serialization operation.
         */
        class EncoderContinuation {
            public:
                EncoderContinuation(Stream* stream) : stream(stream) {}

                /**
                 * @brief Serialization
                 *
                 * @param serializable Serializable object to encode
                 */
                EncoderContinuation& operator<<(const Serializable& serializable);
                /**
                 * @brief Deserialization
                 *
                 * @param serializable Object to store the decode
                 */
                EncoderContinuation& operator>>(Serializable& serializable);
            private:

                /**
                 * @brief Stored stream to send the data to
                 */
                Stream* stream;
                /**
                 * @brief 
                 */
                uint8_t tmp_buffer[8] = {0};
        };
    }
    /**
     * @brief 
     */
    ___impl::EncoderContinuation operator<<(Stream& stream, const Serializable& serializable);
    /**
     * @brief 
     */
    ___impl::EncoderContinuation operator>>(Stream& stream, Serializable& serializable);

    /**
     *  @brief Base serialization class
     *
     */
    class Serializable {
        public:
            /**
             * @brief 
             */
            virtual void serialize(Stream& stream) const = 0;
            /**
             * @brief 
             */
            virtual size_t size() const = 0;
            /**
             * @brief 
             */
            virtual void deserialize(Stream& stream) = 0;
    };
}

#endif // BASE_128_HPP
