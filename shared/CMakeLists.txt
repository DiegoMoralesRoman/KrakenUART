set(HEADERS primitives.hpp messages.hpp states.hpp)
set(SOURCES primitives.cpp messages.cpp states.cpp)
add_library(protocol ${HEADERS} ${SOURCES})

