cmake_minimum_required(VERSION 3.10)

project(ServerTCP)

find_package(fmt CONFIG REQUIRED)

add_executable(Main Main.cpp "Logger.h" "ServerTCP.cpp")

target_link_libraries(Main PRIVATE fmt::fmt)