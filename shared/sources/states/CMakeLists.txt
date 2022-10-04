set(SOURCES test.cpp)
add_library(states ${SOURCES})
target_link_libraries(states protocol)
