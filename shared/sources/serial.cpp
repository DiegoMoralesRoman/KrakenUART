#include "../headers/serial.hpp"

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

Stream& protocol::serial::operator<<(Stream &stream, const Serializable &serializable) {
    serializable.serialize(stream);
    return stream;
}

Stream& protocol::serial::operator>>(Stream &stream, Serializable &serializable) {
    serializable.deserialize(stream);
    return stream;
}

