cmake_minimum_required(VERSION 3.8)

project(ClientTCP)

add_subdirectory("../TcpUtils" "${CMAKE_BINARY_DIR}/TcpUtils_build")
 
add_executable(ClientTCP "ClientTCP/ClientTCP.cpp" "ClientTCP/ClientTCP.h")

target_include_directories(ClientTCP PRIVATE
    "../TcpUtils/TcpUtils"
)

target_link_libraries(ClientTCP PRIVATE TcpUtils)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ClientTCP PROPERTY CXX_STANDARD 20)
endif()
