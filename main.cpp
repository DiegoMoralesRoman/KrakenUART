#include <stdio.h>
#include "shared/headers/protocol.hpp"
#include "shared/headers/utils.hpp"

#include <iostream>


ProtocolSM sm1, sm2;

class SerialBuffer : virtual public ProtocolBuffer {
    public:
        bool check_hash(uahruart::utils::hash_t hash) override {
            return true;
        }


        ProtocolBuffer& operator<<(const protocol::serial::Serializable& serializable) override {
            printf("Writing\n");
            (tx_buffer + ammount_written) << serializable;

            // Send to connected buffer
            conn_buffer->receive(tx_buffer + ammount_written, serializable.size());
            ammount_written += serializable.size();
            printf("Ammount written: %lu\n", ammount_written);
            return *this;
        }


        const ProtocolBuffer& operator>>(protocol::serial::Serializable& serializable) const override {
            printf("Reading\n");
            (rx_buffer + actually_read) >> serializable;
            actually_read += serializable.size();
            return *this;
        }

        void receive(const char* buffer, size_t ammount) {
            printf("Received ammount: %lu\n", ammount);
            // Copy to rx buffer
            for (size_t i = 0; i < ammount; i++) {
                rx_buffer[i + ammount_read] = buffer[i];
            }
            ammount_read += ammount;
            print_rx();
            m_callbacks[BYTES_RCV]();
        }

        void print_tx() {
            for (size_t i = 0; i < sizeof(tx_buffer); i++)
                printf("%i ", static_cast<int>(tx_buffer[i]));
        }

        void print_rx() {
            for (size_t i = 0; i < sizeof(rx_buffer); i++)
                printf("%i ", static_cast<int>(rx_buffer[i]));
        }

        uahruart::utils::hash_t calculate_hash() override {
            return uahruart::utils::calculate_hash(rx_buffer, ammount_read);
        }

        const char *const beginning_rx() override {return rx_buffer;}
        const char *const beginning_tx() override {return tx_buffer;}

    public: // Theoretically private
        mutable size_t ammount_read = 0; // This variable would be automatically used from the serial implementation
        mutable size_t actually_read = 0;
        size_t ammount_written = 0;
        char tx_buffer[64];
        mutable char rx_buffer[64];
        SerialBuffer* conn_buffer = nullptr;
} sm1_buffer, sm2_buffer;


int main() {
    // protocol::primitives::Int32 test_val = 123456;
    // sm1.set_state(&sm1.ctx.s_idle);
    // sm1.ctx.buffer = &sm1_buffer;
    // sm1 << test_val;
    // printf("Written: %lu\n", sm1_buffer.ammount_written);
    // // Read back value
    // protocol::primitives::Int32 rec_val, hash, tmp;
    // (sm1_buffer.tx_buffer + protocol::messages::Header::static_size()) >> tmp >> rec_val >> hash;
    // printf("Read value: %lu, hash: %lu\n", static_cast<size_t>(rec_val), static_cast<size_t>(hash));
    // sm1_buffer.print_tx();
    // std::cout << "Written: " << sm1_buffer.ammount_written << '\n';
    // std::cout << "Hash: " << uahruart::utils::calculate_hash(sm1_buffer.tx_buffer, sm1_buffer.ammount_written - protocol::primitives::Checksum::static_size()) << '\n';
    //
    sm1_buffer.conn_buffer = &sm2_buffer;
    sm2_buffer.conn_buffer = &sm1_buffer;

    protocol::primitives::Int32 message = 123456;
    sm1.set_state(&sm1.ctx.s_idle);
    sm1.ctx.buffer = &sm1_buffer;

    sm2.ctx.buffer = &sm2_buffer;
    sm2.set_state(&sm2.ctx.s_idle);

    sm2.ctx.buffer->on(protocol::states::ProtocolBuffer::BYTES_RCV, []() -> void {
        std::cout << "Mandando señal" << std::endl;
        sm2.ctx.ammount_read = sm2_buffer.ammount_read;
        sm2.signal(signals::BYTES_RCV);
    });
    
    sm1 << message;

    sm2_buffer.print_rx();
}
