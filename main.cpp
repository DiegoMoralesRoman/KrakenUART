#include <stdio.h>
#include "shared/headers/protocol.hpp"
#include "shared/headers/utils.hpp"

#include <iostream>


ProtocolSM sm1;
void sm1_rcv_bytes(const char* buffer, size_t ammount) {
    sm1.ctx.current_buffer = const_cast<char*>(buffer);
    sm1.ctx.ammount_read = ammount;
    sm1.signal(signals::BYTES_RCV);
}

class SerialBuffer : virtual public ProtocolBuffer {
    public:
        bool check_hash(uahruart::utils::hash_t hash) override {
            return true;
        }


        ProtocolBuffer& operator<<(const protocol::serial::Serializable& serializable) override {
            printf("Writing\n");
            ammount_written += serializable.size();
            (tx_buffer + ammount_written) << serializable;
            return *this;
        }


        const ProtocolBuffer& operator>>(protocol::serial::Serializable& serializable) const override {
            printf("Reading\n");
            ammount_read += serializable.size();
            (rx_buffer + ammount_read) >> serializable;
            return *this;
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
        mutable size_t ammount_read = 0;
        size_t ammount_written = 0;
        char tx_buffer[64];
        mutable char rx_buffer[64];
} sm1_buffer;


int main() {
    protocol::primitives::Int32 test_val = 123456;
    sm1.set_state(&sm1.ctx.s_idle);
    sm1.ctx.buffer = &sm1_buffer;
    sm1 << test_val;
    printf("Written: %lu\n", sm1_buffer.ammount_written);
    // Read back value
    protocol::primitives::Int32 rec_val, hash, tmp;
    (sm1_buffer.tx_buffer + protocol::messages::Header::static_size()) >> tmp >> rec_val >> hash;
    printf("Read value: %lu, hash: %lu\n", static_cast<size_t>(rec_val), static_cast<size_t>(hash));
    sm1_buffer.print_tx();
    std::cout << "Written: " << sm1_buffer.ammount_written << '\n';
    std::cout << "Hash: " << uahruart::utils::calculate_hash(sm1_buffer.tx_buffer, sm1_buffer.ammount_written - protocol::primitives::Checksum::static_size()) << '\n';
}
