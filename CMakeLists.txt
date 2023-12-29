cmake_minimum_required(VERSION 3.18)
project(POS_SP C)

set(CMAKE_C_STANDARD 11)

add_executable(POS_SP main.c
        server/moderator.h server/moderator.c
        server/sockety/passive_socket.h server/sockety/passive_socket.c)

target_link_libraries(POS_SP pthread)