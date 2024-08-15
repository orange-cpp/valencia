add_library(asio INTERFACE)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE)
target_include_directories(asio INTERFACE asio/asio/include)

add_subdirectory(glfw)

project(nativefiledialog)

add_library(nativefiledialog STATIC nativefiledialog/src/nfd_common.c)
# add specific implementations
if (WIN32)
    target_sources(nativefiledialog PRIVATE nativefiledialog/src/nfd_win.cpp)
elseif (APPLE)
    target_sources(nativefiledialog PRIVATE nativefiledialog/src/nfd_cocoa.m)
elseif (UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK REQUIRED gtk+-3.0)

    include_directories(${GTK_INCLUDE_DIRS})
    link_directories(${GTK_LIBRARY_DIRS})

    target_link_libraries(nativefiledialog PRIVATE ${GTK_LIBRARIES})
    target_sources(nativefiledialog PRIVATE nativefiledialog/src/nfd_gtk.c)
endif ()

target_include_directories(nativefiledialog PUBLIC nativefiledialog/src/include)


project(imgui)

add_library(imgui STATIC imgui/imgui.cpp)
target_include_directories(imgui PUBLIC imgui imgui/backends)
target_link_libraries(imgui PUBLIC glfw)
target_sources(imgui PRIVATE imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)