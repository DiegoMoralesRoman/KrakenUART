set(HEADERS primitives.hpp messages.hpp states.hpp utils.hpp base128.hpp)
set(SOURCES primitives.cpp messages.cpp states.cpp utils.cpp base128.cpp)

list(TRANSFORM HEADERS PREPEND headers/)
list(TRANSFORM SOURCES PREPEND sources/)

add_subdirectory(sources/states)

add_library(protocol ${HEADERS} ${SOURCES})
target_link_libraries(protocol states)

