#include <stdio.h>
#include "server/sockety/passive_socket.h"

int main(int argc, char** argv) {

    PASSIVE_SOCKET sock;
    passive_socket_init(&sock);
    passive_socket_start_listening(&sock, 12345);
    while (passive_socket_is_listening(&sock)) {
        printf("Server is listening to connections!\n");
    }
    passive_socket_stop_listening(&sock);
    passive_socket_destroy(&sock);

    return 0;
}
