#include <iostream>

#include "shared/headers/primitives.hpp"
#include "shared/headers/messages.hpp"
#include "shared/headers/states.hpp"

#include <thread>
#include <mutex>
#include <queue>

// FAIL TEST
size_t sm1_fail_message = 1;
size_t sm2_fail_message = 2;
// ---------

std::queue<uint8_t> sm_queue, sm2_queue;

std::mutex sm_mtx, sm2_mtx;

protocol::states::StateMachine sm;
protocol::states::StateMachine sm2;

void sm_send_bytes(const char* msg, size_t len) {
    std::cout << "\033[33mFROM SM1:\033[0m " << std::hex;

    // CORRUPT MESSAGE
    bool fail = false;
    if (sm1_fail_message-- == 0) {
        std::cout << "\nCORRUPTING MESSAGE\n";
        fail = true;
    }


    for (size_t i = 0; i < len; i++) {
        if (fail) {
            fail = false;
            sm2_queue.push(~msg[i]);
        } else {
            sm2_queue.push(msg[i]);
        }
        std::cout << static_cast<uint16_t>(msg[i]) << ' ';
    }
    std::cout << std::dec << '\n';

    sm2_mtx.unlock();
}

void sm2_send_bytes(const char* msg, size_t len) {
    std::cout << "\033[32mFROM SM2:\033[0m " << std::hex;

    // CORRUPT MESSAGE
    bool fail = false;
    if (sm2_fail_message-- == 0) {
        std::cout << "\nCORRUPTING MESSAGE\n";
        fail = true;
    }

    for (size_t i = 0; i < len; i++) {
        if (fail) {
            fail = false;
            sm_queue.push(~msg[i]);
        } else {
            sm_queue.push(msg[i]);
        }
        std::cout << static_cast<uint16_t>(msg[i]) << ' ';
    }
    std::cout << std::dec << '\n'; 

    sm_mtx.unlock();
}

void sm_reader() {
    while (true) {
        sm_mtx.lock();
        while (!sm_queue.empty()) {
            sm.parse_byte(sm_queue.front());
            sm_queue.pop();
        }
    }
}

void sm2_reader() {
    while (true) {
        sm2_mtx.lock();
        while (!sm2_queue.empty()) {
            sm2.parse_byte(sm2_queue.front());
            sm2_queue.pop();
        }
    }
}

int main() {

    // Print ack
    std::cout << "ACKs:\n" << std::hex;

    std::cout << "HEADER_ACK: " << protocol::messages::HEADER_ACK << '\n';
    std::cout << "HEADER_NACK: " << protocol::messages::HEADER_NACK << '\n';
    std::cout << "BODY_ACK: " << protocol::messages::BODY_ACK << '\n';
    std::cout << "BODY_NACK: " << protocol::messages::BODY_NACK << '\n';
    std::cout << "UNDEF_ACK: " << protocol::messages::UNDEF_ACK << '\n';
    std::cout << "CORR_ACK: " << protocol::messages::CORR_ACK << '\n';

    sm.m_send_buffer = sm_send_bytes;
    sm2.m_send_buffer = sm2_send_bytes;

    auto sm1_worker = std::thread(sm_reader);
    auto sm2_worker = std::thread(sm2_reader);

    protocol::messages::Admin msg;
    msg.ack = 123;

    sm.send_message(std::move(msg));
    
    // for (size_t i = 0; i < msg.size(); i++) {
    //     sm.parse_byte(buff[i]);
    // }

    sm1_worker.join();
    sm2_worker.join();

    return 0;
}
