set(HEADERS primitives.hpp messages.hpp states.hpp utils.hpp serial.hpp rmi.hpp message_identification.hpp msg_ids.hpp)
set(SOURCES primitives.cpp messages.cpp utils.cpp serial.cpp rmi.cpp message_identification.cpp)

list(TRANSFORM HEADERS PREPEND headers/)
list(TRANSFORM SOURCES PREPEND sources/)

add_subdirectory(sources/states)

add_library(protocol ${HEADERS} ${SOURCES})
target_link_libraries(protocol states)

