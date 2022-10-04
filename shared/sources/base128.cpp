#include "../headers/base128.hpp"

using namespace protocol::base128;

___impl::EncoderContinuation& ___impl::EncoderContinuation::operator<<(const Serializable &serializable) {
    return *this;
}

___impl::EncoderContinuation& ___impl::EncoderContinuation::operator>>(Serializable &serializable) {
    return *this;
}

___impl::EncoderContinuation protocol::base128::operator<<(Stream &stream, const Serializable &serializable) {
    serializable.serialize(stream);
    stream.flush();
    return {&stream};
}

___impl::EncoderContinuation protocol::base128::operator>>(Stream &stream, Serializable &serializable) {
    serializable.deserialize(stream);
    return {&stream};
}
