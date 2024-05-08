cmake_minimum_required(VERSION 3.5)

project(valencia)

set(CMAKE_CXX_STANDARD 26)

add_executable(valencia source/main.cpp)
target_link_libraries(valencia PRIVATE asio)
add_subdirectory(extlibs)