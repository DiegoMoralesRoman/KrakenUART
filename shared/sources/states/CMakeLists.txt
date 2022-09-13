set(SOURCES reading_body.cpp reading_header.cpp reading_trailing.cpp waiting_header_ack.cpp waiting_body_ack)
add_library(states ${SOURCES})
target_link_libraries(states protocol)
