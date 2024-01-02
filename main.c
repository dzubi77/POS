#include <stdio.h>
#include "server/sockety/passive_socket.h"
#include "server/sockety/active_socket.h"
#include "klient/sockety/client_socket.h"

#include "server/moderator.h"
#include "klient/hrac.h"

void server(int port) {
    //server sockets init
    PASSIVE_SOCKET p_sock;
    ACTIVE_SOCKET a_sock;
    passive_socket_init(&p_sock);
    active_socket_init(&a_sock);
    passive_socket_start_listening(&p_sock, port);

    //sync mechanism init
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t otazkaPripravena = PTHREAD_COND_INITIALIZER;
    pthread_cond_t odpovedPripravena = PTHREAD_COND_INITIALIZER;

    //moderator init
    MODERATOR moderator = { 3, 5, &mutex, &otazkaPripravena, &odpovedPripravena };
    pthread_t th_moderator;
    pthread_create(&th_moderator, NULL, moderuj, &moderator);
    passive_socket_wait_for_client(&p_sock, &a_sock);

    printf("Destroying sockets.\n");
    active_socket_destroy(&a_sock);
    passive_socket_destroy(&p_sock);
}

int klient(char* hostname, char* port) {
    CLIENT_SOCKET c_sock;

    // Try to connect to the server
    if (clientInitialize(&c_sock, hostname, port) == 0 && clientConnectToServer(&c_sock) == 0) {
        // Connection to the server successful
        printf("Connected to the server.\n");

        HRAC hrac;
        pthread_t th_hrac;
        pthread_create(&th_hrac, NULL, hraj, &hrac);

        clientCloseConnection(&c_sock);
    } else {
        printf("Connection not established.\n");
        return 1;
    }
    return 0;
}

int main(int argc, char** argv) {

    CLIENT_SOCKET c_sock;

    // Try to connect to the server
    if (clientInitialize(&c_sock, argv[1], argv[2]) == 0 && clientConnectToServer(&c_sock) == 0) {
        printf("Connected to the server.\n");
        clientCloseConnection(&c_sock);
        klient(argv[1], argv[2]);
    } else {
        printf("Connection not established.Creating server.\n");
        server(atoi(argv[2]));
        klient(argv[1], argv[2]);
    }
    return 0;
}




