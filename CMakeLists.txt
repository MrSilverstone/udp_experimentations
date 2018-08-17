cmake_minimum_required(VERSION 2.8)

project(udp)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++14 -g -g3")

add_subdirectory(network)
add_subdirectory(server)
add_subdirectory(client)