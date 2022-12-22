#include "../headers/serial.hpp"
#include "../headers/primitives.hpp"

using namespace protocol::serial;

// Internal functions 
template<typename T>
__always_inline decltype(auto) mod(T a, T b) {
    return (a%b+b)%b;
}

template<typename T>
__always_inline T min(T a, T b) {
    return (a < b) ? a:b;
}

// ==================================================
// Serialization and deserialization buffer wrappers
// ==================================================
___impl::SerializationWrapper::operator char *() {
    return buffer + base_position;
}
// Serialization operatos
___impl::SerializationWrapper protocol::serial::operator<<(char *const buffer, const protocol::serial::Serializable& serializable) {
    serializable.serialize(buffer);
    return {
        buffer,
        serializable.size()
    };
}

___impl::SerializationWrapper& protocol::serial::operator<<(___impl::SerializationWrapper&& wrapper, const protocol::serial::Serializable& serializable) {
    serializable.serialize(wrapper.buffer + wrapper.base_position);
    wrapper.base_position += serializable.size();

    return wrapper;
}

// Deserialization operators
___impl::SerializationWrapper protocol::serial::operator>>(const char *const buffer, protocol::serial::Serializable& serializable) {
    serializable.deserialize(buffer);
    return {
        const_cast<char*>(buffer),
        serializable.size()
    };
}

___impl::SerializationWrapper& protocol::serial::operator>>(___impl::SerializationWrapper&& wrapper, protocol::serial::Serializable& serializable) {
    serializable.deserialize(wrapper.buffer + wrapper.base_position);
    wrapper.base_position += serializable.size();

    return wrapper;
}

