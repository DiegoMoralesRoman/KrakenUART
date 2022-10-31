#include "../headers/serial.hpp"

using namespace protocol::serial;

// Internal functions 
// Real modulo operator
long mod(long a, long b)
{ return (a%b+b)%b; }

// Basic stream operators
Stream& protocol::serial::operator<<(Stream &stream, const Serializable &serializable) {
    serializable.serialize(stream);
    return stream;
}

Stream& protocol::serial::operator>>(Stream &stream, Serializable &serializable) {
    serializable.deserialize(stream);
    return stream;
}

// ==================================================
// Base 128 implementation
// ==================================================

void Base128Stream::set_connection_stream(Stream *stream) {
    m_conn_stream = stream;
}

const uint8_t masks[] = {
    0b00000000,
    0b10000000,
    0b11000000,
    0b11100000,
    0b11110000,
    0b11111000,
    0b11111100,
    0b11111110,
};

template<typename T>
__always_inline T min(T a, T b) {
    return (a < b) ? a:b;
}
// Base128 stream implementation
#include <iostream>
void Base128Stream::read(char *buff, const size_t ammount) {
    size_t encoded_size = (ammount * 8) / 7;

    uint8_t relative_index = m_bytes_rw % 8;

    bool prev_grp_included_flag = false;
    if ((ammount * 8) % 7) {
        ++encoded_size; 
        prev_grp_included_flag = true;
    }

    // if (prev_grp_included) {
    //     --encoded_size;
    // }

    // Read full sized blocks (8 7bit groups)
    size_t full_blocks = encoded_size / 8;
    uint8_t remaining = encoded_size % 8;
    

    const auto read_bytes = [this, relative_index](uint8_t from_start, uint8_t until_end) -> void {
        if (this->prev_grp_included) {
            // Read values depending on the ammount
            this->m_conn_stream->read(this->m_tmp_buffer + ((relative_index + 1) % 8), until_end - 1);
            this->m_conn_stream->read(this->m_tmp_buffer, from_start + 1); // TODO: add one 

            this->m_tmp_buffer[relative_index] = last_byte_read;

            this->prev_grp_included = false;
        } else {
            this->m_conn_stream->read(this->m_tmp_buffer + relative_index, until_end);
            this->m_conn_stream->read(this->m_tmp_buffer, from_start);
        }
        for (uint8_t i = 0; i < 8; i++) this->m_tmp_buffer[i] -= 0x21;
    };

    m_buff_pos = relative_index; // Update current buffer position
    
    // Read full blocks
    m_bytes_pending_processing = 8;
    uint8_t from_start = 8 - relative_index;
    uint8_t until_end = relative_index;
    for (size_t i = 0; i < full_blocks; i++) {
        // Copy data into buffer
        read_bytes(from_start, until_end);
        // Subtrace 0x21 from all bytes
        decode(buff);
    }
    last_byte_read = m_tmp_buffer[relative_index] + 0x21;
    m_bytes_rw += 8 * full_blocks;

    // Copy remaining data to memory
    // [] [] [] [] [] [] [] [] 
    //          ^
    until_end = min(remaining, static_cast<uint8_t>(8 - relative_index));
    from_start = (until_end < remaining) ? (remaining - until_end):0;

    // Read data into buffer
    read_bytes(from_start, until_end);

    // Flush data
    m_bytes_pending_processing = remaining;
    decode(buff);
    m_bytes_rw += remaining;
    last_byte_read = m_tmp_buffer[m_bytes_rw % 8] + 0x21;

    // Update flags
    prev_grp_included = prev_grp_included_flag;
    if (prev_grp_included_flag)
        --m_bytes_rw;
}

void Base128Stream::decode(char* buffer) {
    uint8_t decoded_size = (m_bytes_pending_processing * 7) / 8;
    // uint8_t decoded_size = m_bytes_pending_processing;
    for (uint8_t i = m_bytes_rw; i < (m_bytes_rw + decoded_size); i++) {
        const uint8_t start_grp = (((i % 8) * 8) / 7) % 8;
        const uint8_t end_grp = (start_grp + 1) % 8;

        const uint8_t start_grp_pos = (i * 8 - start_grp * 7) % 8;
        const uint8_t end_grp_pos = 0;
        const uint8_t start_grp_size = 7 - start_grp_pos;
        const uint8_t end_grp_size = 8 - start_grp_size;
        // Get 7bit groups
        uint8_t start_data = m_tmp_buffer[start_grp];
        uint8_t end_data = m_tmp_buffer[end_grp];

        i -= m_bytes_rw;

        buffer[i] = start_data << (end_grp_size);
        buffer[i] |= end_data >> (7 - end_grp_size);

        std::cout << "end: " << static_cast<int>(end_data + 0x21) << ", start: " << static_cast<int>(start_data + 0x21) << ", ";
        std::cout << "Decoded value: " << static_cast<int>(buffer[i]) << ": " << buffer[i] << '\n'; 
        i += m_bytes_rw;
    }
}

void Base128Stream::write(const char *buff, size_t ammount) {
    if (last_action != WRITING)
        flush();
    last_action = WRITING;
    // implementation
    for (size_t i = 0; i < ammount; i++) {
        ++m_bytes_rw;
        m_tmp_buffer[m_buff_pos] = buff[i];
        ++m_buff_pos %= 7;

        std::cout << "Encoding: " << static_cast<int>(buff[i]) << '\n';

        if (m_bytes_rw == 7) {
            encode();
        }
    }
}

// Assumes it's using a circular buffer (only for the encode and decode functions)
uint8_t read_7bit_group(char* array, uint8_t index) {

    uint8_t bit_position = 7 * index;
    // Starting byte
    char start = array[bit_position / 8];
    char end = array[((bit_position + 7) / 8) % 7];

    // Get bits in each byte
    uint8_t end_bits = (bit_position + 7) - (bit_position + 7) / 8 * 8;
    uint8_t start_bits = 7 - end_bits;
    uint8_t start_bit = bit_position % 8;

    // Get start bits
    // uint8_t result = 0;
    uint8_t result = start & (masks[start_bits] >> start_bit);
    result <<= end_bits + 1;
    result |= (end & masks[end_bits]) >> start_bits;
    return result >> 1;
}

#include <stdio.h>
void Base128Stream::encode() {
    uint8_t encoded_size = (m_bytes_rw * 8) / 7;
    if ((m_bytes_rw * 8) % 7) ++encoded_size;

    char array[encoded_size];

    for (size_t i = 0; i < encoded_size; i++) {
        // Read neccessary bytes 
        uint8_t value = read_7bit_group(m_tmp_buffer, (m_last_7bit_pos + i) % 8);
        value += 0x21;
        array[i] = value;
    }

    m_last_7bit_pos = (m_last_7bit_pos + encoded_size) % 8;
    m_conn_stream->write(array, encoded_size);

    m_bytes_rw = 0;
    m_buff_pos = 0;
}

void Base128Stream::flush() {
    switch (last_action) {
        case READING: {
            // decode();
            break;
        }
        case WRITING: {
            encode();
            break;
        } 
        case UNDEFINED: {
            break;
        }
    }
    m_bytes_rw = 0;
    m_last_7bit_pos = 0;
    m_buff_pos = 0;
    m_conn_stream->flush();
}
