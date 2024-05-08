add_library(asio INTERFACE)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE)
target_include_directories(asio INTERFACE asio/asio/include)