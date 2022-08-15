set(HEADERS primitives.hpp messages.hpp states.hpp utils.hpp)
set(SOURCES primitives.cpp messages.cpp states.cpp utils.cpp)

add_subdirectory(states)

add_library(protocol ${HEADERS} ${SOURCES})
target_link_libraries(protocol states)

