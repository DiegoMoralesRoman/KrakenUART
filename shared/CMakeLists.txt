set(HEADERS primitives.hpp messages.hpp states.hpp utils.hpp serial.hpp)
set(SOURCES primitives.cpp messages.cpp states.cpp utils.cpp serial.cpp)

list(TRANSFORM HEADERS PREPEND headers/)
list(TRANSFORM SOURCES PREPEND sources/)

add_subdirectory(sources/states)

add_library(protocol ${HEADERS} ${SOURCES})
target_link_libraries(protocol states)

