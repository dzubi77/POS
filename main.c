#include <stdio.h>
#include "server/sockety/passive_socket.h"
#include "server/sockety/active_socket.h"
#include "klient/sockety/client_socket.h"

#include "server/moderator.h"
#include "klient/hrac.h"

typedef struct server_data {
    int port;
    PASSIVE_SOCKET p_sock;
    ACTIVE_SOCKET a_sock;
} SERVER_DATA;

void initializeSockets(SERVER_DATA *data) {
    passive_socket_init(&(data->p_sock));
    active_socket_init(&(data->a_sock));
    passive_socket_start_listening(&(data->p_sock), data->port);
}

void destroySockets(SERVER_DATA *data) {
    printf("Destroying sockets.\n");
    passive_socket_stop_listening(&(data->p_sock));
    active_socket_destroy(&(data->a_sock));
    passive_socket_destroy(&(data->p_sock));
}

typedef struct klient_data {
    char* port;
    char* hostname;
} KLIENT_DATA;

void* serveruj(void* data) {
    SERVER_DATA *d = (SERVER_DATA *)data;
    //server sockets init
    PASSIVE_SOCKET p_sock;
    ACTIVE_SOCKET a_sock;
    passive_socket_init(&p_sock);
    active_socket_init(&a_sock);
    passive_socket_start_listening(&p_sock, d->port);

    //sync mechanism init
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t otazkaPripravena = PTHREAD_COND_INITIALIZER;
    pthread_cond_t odpovedPripravena = PTHREAD_COND_INITIALIZER;

//    //moderator init
//    MODERATOR moderator = { 0, 5, &mutex, &otazkaPripravena, &odpovedPripravena };
//    pthread_t th_moderator;
//    pthread_create(&th_moderator, NULL, moderuj, &moderator);


    printf("Destroying sockets.\n");
    active_socket_destroy(&a_sock);
    passive_socket_destroy(&p_sock);
}

void* klientuj(void* data) {
    KLIENT_DATA *d = (KLIENT_DATA *)data;
    CLIENT_SOCKET c_sock;

    // Try to connect to the server
    if (clientInitialize(&c_sock, d->hostname, d->port) == 0 && clientConnectToServer(&c_sock) == 0) {
        // Connection to the server successful
        printf("Connected to the server.\n");

//        HRAC hrac;
//        pthread_t th_hrac;
//        pthread_create(&th_hrac, NULL, hraj, &hrac);
        printf("zadaj nieco\n");
        char input[100];
        scanf("%s", input);
        clientCloseConnection(&c_sock);
        printf("Client disconnected.\n");
    } else {
        printf("Connection not established.\n");
    }
    return 0;
}

int main(int argc, char** argv) {

    CLIENT_SOCKET c_sock;

    // skusi sa pripojit ako klient
    if (clientInitialize(&c_sock, argv[1], argv[2]) == 0 && clientConnectToServer(&c_sock) == 0) {
        printf("Connected to the server.\n");
        clientCloseConnection(&c_sock);

        //server existuje, vytvorenie klienta
        KLIENT_DATA klient_data;
        pthread_t th_klient;
        klient_data.port = argv[2];
        klient_data.hostname = argv[1];
        pthread_create(&th_klient, NULL, klientuj, &klient_data);

        pthread_join(th_klient, NULL);
    } else {
        //server nie je zalozeny, treba ho zalozit
        printf("Connection not established.Creating server.\n");

        //vytvorenie servera
        SERVER_DATA server_data;
        server_data.port = atoi(argv[2]);
        initializeSockets(&server_data);

        //vytvorenie klienta
        pthread_t th_klient;
        KLIENT_DATA klient_data;
        klient_data.port = argv[2];
        klient_data.hostname = argv[1];
        pthread_create(&th_klient, NULL, klientuj, &klient_data);

        pthread_join(th_klient, NULL);
        destroySockets(&server_data);
    }
    return 0;
}




