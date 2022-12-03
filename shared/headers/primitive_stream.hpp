#ifndef PRIMITIVE_STREAM_HPP
#define PRIMITIVE_STREAM_HPP


#include "primitives.hpp"
#include "serial.hpp"

namespace protocol::serial {
    class PrimitiveStream : Stream {
        public:
            void propagate(const PropagationContext &ctx) override;
            void flush() override;
    };
};

#endif // PRIMITIVE_STREAM_HPP
