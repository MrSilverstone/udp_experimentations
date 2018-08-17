include_directories(../network)

add_executable(server main.cpp
  Server.cpp)

target_link_libraries(server network)