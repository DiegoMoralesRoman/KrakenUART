#include "../headers/message_identification.hpp"

using namespace uahruart::protocol::id;

template<typename... Types>
struct TypeArray {

};

#include <iostream>

CallbackStore::InvokersArr_t CallbackStore::m_invokers = []<typename... Types>(TypeArray<Types...>) -> InvokersArr_t {
    return InvokersArr_t{[](functor<void()>& untyped, const char *const buffer) {
        // Get value
        Types serializable;
        buffer >> serializable;
        reinterpret_cast<functor<void(const Types&)>*>(&untyped)->operator()(serializable);
    }...};
}(TypeArray<
    uahruart::protocol::primitives::Int8,
    uahruart::protocol::primitives::Int16,
    uahruart::protocol::primitives::Int32,
    uahruart::protocol::primitives::String,

    uahruart::protocol::messages::Test
>{});
