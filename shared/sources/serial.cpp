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

// ##################################################
// Basic stream operators
// ##################################################

// Stream& protocol::serial::operator<<(Stream &stream, const Serializable &serializable) {
//     serializable.serialize(stream);
//     return stream;
// }

// Stream& protocol::serial::operator>>(Stream &stream, Serializable &serializable) {
//     serializable.deserialize(stream);
//     return stream;
// }

// ==================================================
// Serialization and deserialization buffer wrappers
// ==================================================
___impl::SerializationWrapper::operator char *() {
    return buffer;
}
// Serialization operatos
___impl::SerializationWrapper operator<<(char *const buffer, const protocol::serial::Serializable& serializable) {
    serializable.serialize(buffer);
    return {
        buffer,
        serializable.size()
    };
}

___impl::SerializationWrapper& operator<<(___impl::SerializationWrapper& wrapper, const protocol::serial::Serializable& serializable) {
    wrapper.base_position += serializable.size();
    serializable.serialize(wrapper.buffer + wrapper.base_position);

    return wrapper;
}

// Deserialization operators
___impl::SerializationWrapper operator>>(char *const buffer, protocol::serial::Serializable& serializable) {
    serializable.deserialize(buffer);
    return {
        buffer,
        serializable.size()
    };
}

___impl::SerializationWrapper& operator>>(___impl::SerializationWrapper& wrapper, protocol::serial::Serializable& serializable) {
    wrapper.base_position += serializable.size();
    serializable.deserialize(wrapper.buffer + wrapper.base_position);

    return wrapper;
}

