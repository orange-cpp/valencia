cmake_minimum_required(VERSION 3.26)

project(valencia LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/${CMAKE_BUILD_TYPE}")

add_executable(valencia source/main.cpp)

target_link_libraries(valencia PRIVATE asio nativefiledialog glfw imgui opengl32)
add_subdirectory(extlibs)
add_subdirectory(source)