set(HEADERS primitives.hpp messages.hpp states.hpp utils.hpp)
set(SOURCES primitives.cpp messages.cpp states.cpp utils.cpp)
add_library(protocol ${HEADERS} ${SOURCES})

