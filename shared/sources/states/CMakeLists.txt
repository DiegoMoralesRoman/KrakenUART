set(SOURCES protocol_states.cpp idle.cpp reading_header.cpp)
add_library(states ${SOURCES})
target_link_libraries(states protocol)
