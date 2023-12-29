//
// Created by matdz on 12/27/2023.
//

#include "moderator.h"
#include "..//kviz.h"
#include "sockety/passive_socket.h"
#include "sockety/active_socket.h"

int main(int argc, char** argv) {

    PASSIVE_SOCKET passiveSocket;
    passive_socket_init(&passiveSocket);
    passive_socket_start_listening(&passiveSocket, 12345);


    return 0;
}
