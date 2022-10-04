cmake_minimum_required(VERSION 3.10)

project(ProtocolV3)

# Specify standatd
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")

# Libraries
add_subdirectory(shared)

add_compile_options(-Wall)
add_executable(ProtocolV3 main.cpp rmi.cpp)
target_link_libraries(ProtocolV3 protocol -lpthread)

