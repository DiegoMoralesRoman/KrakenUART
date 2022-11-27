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
            /**
             * @brief Reads n bytes and stores them in a buffer. It's recommended to return 0 if no data is available.
             * 
             * @param buff Buffer to store the data
             * @param ammount Ammount of bytes to read
             *
             * @return Number of bytes read
             */
            virtual size_t read(char* buff, const size_t ammount) = 0;
            /**
             * @brief Writes n bytes obtained from the passed buffer
             * 
             * @param buff Buffer to get data from
             * @param ammount Ammount of bytes to write
             */
            virtual void write(const char* buff, size_t ammount) = 0;

            /**
             * @brief Flushes the stream and sends or reads everything
             */
            virtual void flush() = 0;
    };

    // ==================================================
    // Base128 encoding and decoding NOT IMPLEMENTED (AND NOT USED)
    // ==================================================

    // Base128 intermediate stream
    /**
     * @brief This class allows encoding and decoding of binary data to and from Base128 encoding.
     * @details
     * This class will read groups of 7 bytes from and to the designed stream **so it will not output in real time**. If the data
     * to be written is not a multiple of 7 (most likely) the flush method will have to be used.
     */
    class Base128Stream : public Stream {
        public:
            // Read and write methods
            virtual size_t read(char* buff, const size_t ammount) override;
            virtual void write(const char* buff, size_t ammount) override;
            /**
             * @brief Flushes the stream (finished reading or finished writing the buffered data)
             * @details
             * This method will be called upon changing the buffer from reading to writing mode or after the serialization of a message.
             * As well as when the buffer has received a full message
             */
            virtual void flush() override;

            /**
             * @brief Set stream to read and write to
             * @param stream IN/OUT stream
             * @details 
             * This stream will be used to read and write the encoded data.
             */
            void set_connection_stream(Stream* stream);

        private:
            enum {
                UNDEFINED,
                READING,
                WRITING
            } last_action = UNDEFINED;
            // Base128 encoding state
            char m_tmp_buffer[8] = {0};

            uint8_t m_buff_pos = 0;
            size_t m_bytes_rw = 0;
            uint8_t m_last_7bit_pos = 0;
            uint8_t m_bytes_pending_processing = 0;
            bool prev_grp_included = false;
            uint8_t last_byte_read = 0;

            void encode();
            void decode(char* buffer);

            Stream* m_conn_stream;
    };

    /**
     * @brief 
     */
    Stream& operator<<(Stream& stream, const Serializable& serializable);
    /**
     * @brief 
     */
    Stream& operator>>(Stream& stream, Serializable& serializable);

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

#endif // SERIAL_HPP
