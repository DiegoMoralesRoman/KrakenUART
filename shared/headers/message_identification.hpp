#ifndef MESSAGE_IDENTIFICATION_HPP
#define MESSAGE_IDENTIFICATION_HPP

#include <stdint.h>
#include "../headers/utils.hpp"
#include <array>

#include "msg_ids.hpp"
// Messages and primitives
#include "../headers/primitives.hpp"
#include "../headers/messages.hpp"

namespace uahruart::protocol::id {
    class CallbackStore {
        public:
            template<typename Callback_Arg>
            void on(const IDs id, functor<void(Callback_Arg&)> callback) {
                auto retreived_id = static_cast<uint16_t>(id);
                // Remove type
                m_callbacks[static_cast<size_t>(retreived_id)] = *reinterpret_cast<functor<void()>*>(&callback);
            }
            
            void invoke(const uint16_t id, const char *const buffer) {
                // Retreive type
                m_invokers[id](m_callbacks[id], buffer);
            }

        private:
            using InvokersArr_t = std::array<void(*)(functor<void()>&, const char* const), (uint16_t)uahruart::protocol::id::IDs::SIZE>;
            static InvokersArr_t m_invokers;
            std::array<functor<void()>, (size_t)IDs::SIZE> m_callbacks;
    };

}

#endif // MESSAGE_IDENTIFICATION_HPP
