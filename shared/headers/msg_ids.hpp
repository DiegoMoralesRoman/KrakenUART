#ifndef MSG_IDS_HPP
#define MSG_IDS_HPP

#include <stdint.h>

namespace uahruart::protocol::id {
    enum class IDs : uint16_t {
        // Primitives
        PRIMITIVE_I8,
        PRIMITIVE_I16,
        PRIMITIVE_I32,
        PRIMITIVE_STR,
        // Messages
        TEST,

        // Do not modify
        SIZE,
        UNDEF
    };
}


#endif
