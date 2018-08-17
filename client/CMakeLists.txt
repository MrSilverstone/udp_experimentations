include_directories(../network)

add_executable(client main.cpp
Client.cpp)

target_link_libraries(client network)