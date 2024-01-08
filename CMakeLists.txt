cmake_minimum_required(VERSION 3.18)
project(POS_SP C)

set(CMAKE_C_STANDARD 11)

add_executable(POS_SP main.c
        server/moderator.h server/moderator.c
        server/sockety/passive_socket.h server/sockety/passive_socket.c
        server/sockety/active_socket.h server/sockety/active_socket.c
        server/sockety/char_buffer.h server/sockety/char_buffer.c
        server/sockety/linked_list.h
        klient/sockety/client_socket.h klient/sockety/client_socket.c
        server/hrac.h server/hrac.c
        server/kviz.h
        server/kviz.c
        klient/klient.c
        klient/klient.h
        server/server.h
        server/server.c
)

target_link_libraries(POS_SP pthread)