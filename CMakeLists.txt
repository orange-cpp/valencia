cmake_minimum_required(VERSION 3.26)

option(USE_CODE_VIRTUALIZER "Use code virtualizer for protection" ON)

project(valencia LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/${CMAKE_BUILD_TYPE}")

if (WIN32)
    add_executable(valencia WIN32 source/main.cpp)
else ()
    add_executable(valencia source/main.cpp)
endif ()

target_link_libraries(valencia PRIVATE asio nativefiledialog glfw imgui cv-sdk)

if (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(valencia PRIVATE ${OPENGL_LIBRARIES})
else (WIN32)
    target_link_libraries(valencia PRIVATE opengl32)
endif ()
target_include_directories(valencia PRIVATE source)
add_subdirectory(extlibs)
add_subdirectory(source)


if (USE_CODE_VIRTUALIZER)
    target_compile_definitions(valencia PRIVATE USE_CODE_VIRTUALIZER)
    target_link_libraries(valencia PRIVATE cv-sdk)
endif ()