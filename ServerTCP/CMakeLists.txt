cmake_minimum_required(VERSION 3.8)

project(ServerTCP)

add_subdirectory("../TcpUtils" "${CMAKE_BINARY_DIR}/TcpUtils_build")
 
add_executable(ServerTCP "ServerTCP/ServerTCP.cpp" "ServerTCP/ServerTCP.h")

target_include_directories(ServerTCP PRIVATE
    "../TcpUtils/TcpUtils"
)

target_link_libraries(ServerTCP PRIVATE TcpUtils)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ServerTCP PROPERTY CXX_STANDARD 20)
endif()
