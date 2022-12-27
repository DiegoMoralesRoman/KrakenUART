#ifndef PROTOCOL_STATES_HPP
#define PROTOCOL_STATES_HPP

#include "serial.hpp"
#include "states.hpp"
#include "utils.hpp"
#include <array>

#include "message_identification.hpp"

namespace uahruart::protocol::states {

    namespace ___impl {struct ProtocolSMContext;}

    namespace signals {
        enum {
            BYTES_RCV,
            DATA_SENT,
            HASH_ERROR
        };
    }

    /**
    * State machine
    */
    using ProtocolSM = StateMachine<___impl::ProtocolSMContext>;

    // Syntax sugar operators
    ProtocolSM& operator<<(ProtocolSM& protocol_sm, const uahruart::protocol::serial::Serializable& serializable);
    const ProtocolSM& operator>>(const ProtocolSM& protocol_sm, uahruart::protocol::serial::Serializable& serializable);

    template class State<___impl::ProtocolSMContext>;
    using ProtocolState = State<___impl::ProtocolSMContext>;

    // ==================================================
    // States definitions
    // ==================================================

    /**
     * State machine diagram (Mermaid diagram)
     * Copy on https://mermaid.live/edit
        graph LR
            idle[Idle]
            rh[Reading Header]
            rb[Reading Body]
            flush[Flush input / ERR]

            idle --"Bytes rcv"--> rh --"Header RCVd OK"--> rb
            rh --"Out of range"--> flush
            rb --"Header read"--> idle

            flush --"Flushed"--> rh

            wack[Waiting ACK]

            idle --"Data sent"--> wack --"NACK"--> wack
            wack --"ACK"--> idle
        

        Example message flow diagram
    
        sequenceDiagram
            participant TX
            participant RX

            TX ->> RX: Header/Body/SUM
            alt invalid
                RX ->> TX: NACK
                note over TX, RX: Defaults to NACK if ACK is corrupted
                TX ->> RX: Header/Body/SUM
            else
                alt pending messages
                    RX ->> TX: Header/Body/SUM
                    RX -> TX: ...
                else
                    RX ->> TX: ACK
                    note over TX, RX: If ACK is corrupted will be NACK
                end
            end
     */

    /**
     * @brief Base state where sending and receiving are possibilities
     */
    class Idle : virtual public ProtocolState {
        public:
            Idle(ProtocolSM* sm) : ProtocolState{sm} {}

            State* on_enter() override;
            void on_exit() override;
            ProtocolState* signal(const Signal_t signal) override;
    };

    /**
     * @brief Reading the header of the message
     * @details Before leaving, after reading the header, the checksum is calculated
     */
    class ReadingHeader : virtual public ProtocolState {
        public:
            ReadingHeader(ProtocolSM* sm) : ProtocolState{sm} {}

            State* on_enter() override;
            void on_exit() override;
            ProtocolState* signal(const Signal_t signal) override;

        private:
            size_t bytes_read = 0;
    };

    class ReadingBody : virtual public ProtocolState {
        public:
            ReadingBody(ProtocolSM* sm) : ProtocolState{sm} {}

            ProtocolState* on_enter() override;
            void on_exit() override;
            ProtocolState* signal(const Signal_t signal) override;
    };

    class WaitingACK : public ProtocolState {
        public:
            WaitingACK(ProtocolSM* sm) : ProtocolState{sm} {}

            ProtocolState* on_enter() override;
            void on_exit() override;
            ProtocolState* signal(const Signal_t signal) override;
    };

    class Error : public ProtocolState {
        public:
            Error(ProtocolSM* sm) : ProtocolState{sm} {}

            ProtocolState* on_enter() override;
            void on_exit() override;
            ProtocolState* signal(const Signal_t signal) override;
    };


    // ==================================================
    // End of state definitions
    // ==================================================
    class ProtocolBuffer {
        public:
            /**
             * @brief Checks buffer to see if the hash coincides with what's in the buffer
             * @param hash Hash to check against
             * @return true if the hash coincides with the provided one
             *
             * @details
             * This method should use the calculate_hash() method that has to be overloaded
             */
            virtual bool check_hash(uahruart::utils::hash_t hash) = 0;

            /**
             * @brief Returns the calculated hash for the ammount of data written to the buffer
             * @return Calculated hash
             */
            virtual uahruart::utils::hash_t calculate_hash() = 0;

            /**
             * @brief Start of the transmit buffer
             */
            virtual const char* const beginning_tx() = 0;

            /**
             * @brief Start of the receive buffer
             */
            virtual const char* const beginning_rx() = 0;
            
            /**
             * @brief Serialization
             * @return Has to return the internal buffer to continue serialization
             */
            virtual ProtocolBuffer& operator<<(const uahruart::protocol::serial::Serializable& serializable) = 0;

            /**
             * @brief Deserialization
             * @return Has to return the internal buffer to continue deserialization
             */
            virtual const ProtocolBuffer& operator>>(uahruart::protocol::serial::Serializable& serializable) const = 0;
            
            enum Actions {
                BYTES_RCV,
                SIZE
            };
            void on(Actions action, const functor<void(size_t)> callback);

            protected:
            std::array<functor<void(size_t)>, Actions::SIZE> m_callbacks = {};
    };

    // State machine context
    namespace ___impl {
        struct ProtocolSMContext;

        /**
         * @brief Protocol state machine context
         */
        struct ProtocolSMContext {
            ProtocolSMContext(ProtocolSM* sm);

            // Global state variables
            size_t ammount_read = 0;
            size_t body_size = 0;
            const char* tagged_buffer_position = nullptr;

            uint16_t message_type = static_cast<uint16_t>(uahruart::protocol::id::IDs::UNDEF);

            uahruart::protocol::id::CallbackStore callback_store;

            // States
            Idle s_idle;
            ReadingHeader s_reading_header;
            ReadingBody s_reading_body;
            WaitingACK s_waiting_ack;
            Error s_error;
            
            // Other variables
            ProtocolSM* sm;
            ProtocolBuffer* buffer;
        };
    }
}

#endif // PROTOCOL_STATES_HPP
