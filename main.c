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
    CLIENT_SOCKET* c_sock;
} KLIENT_DATA;

void* serveruj(void* data) {
    SERVER_DATA *d = (SERVER_DATA *)data;
    passive_socket_wait_for_client(&d->p_sock,&d->a_sock);
    printf("server zaznamenal klienta.\n");
    CHAR_BUFFER output;
    active_socket_start_reading(&d->a_sock);
    active_socket_try_get_read_data(&d->a_sock,&output);
    printf(output.data);
    //sync mechanism init
//    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//    pthread_cond_t otazkaPripravena = PTHREAD_COND_INITIALIZER;
//    pthread_cond_t odpovedPripravena = PTHREAD_COND_INITIALIZER;

    //moderator init
//    MODERATOR moderator = { 0, 5, &mutex, &otazkaPripravena, &odpovedPripravena };
//    pthread_t th_moderator;
//    pthread_create(&th_moderator, NULL, moderuj, &moderator);


    printf("Destroying sockets.\n");
    active_socket_destroy(&d->a_sock);
    passive_socket_destroy(&d->p_sock);
}

void* klientuj(void* data) {
    KLIENT_DATA *d = (KLIENT_DATA *)data;
    printf("zadaj nieco\n");
    char input[100];
    scanf("%s", input);
    clientSendData(d->c_sock,input);
//    clientCloseConnection(&d->c_sock);
//    printf("Client disconnected.\n");

}

int clientTryConnect(KLIENT_DATA* data) {
    if (clientInitialize(data->c_sock, data->hostname, data->port) == 0 && clientConnectToServer(data->c_sock) == 0) {
        printf("Client connected.\n");
        return 0;
    }
    printf("Client not connected.\n");
    return 1;
}

int main(int argc, char** argv) {

    CLIENT_SOCKET c_sock;
    KLIENT_DATA klient_data;
    klient_data.port = argv[2];
    klient_data.hostname = argv[1];
    klient_data.c_sock = &c_sock;

    // skusi sa pripojit ako klient
    if (clientTryConnect(&klient_data) == 0) {
        printf("Connected to the server.\n");

        //server existuje, vytvorenie klienta
        klientuj(&klient_data);
    } else {
        //server nie je zalozeny, treba ho zalozit
        printf("Connection not established.Creating server.\n");

        //vytvorenie servera
        printf("vytvara sa server\n");
        SERVER_DATA server_data;
        server_data.port = atoi(argv[2]);
        initializeSockets(&server_data);
        pthread_t th_server;
        pthread_create(&th_server,NULL,serveruj,&server_data);

        //vytvorenie klienta
        printf("vytvara sa klient.\n");
        pthread_t th_klient;
        sleep(1);
        clientTryConnect(&klient_data);
        pthread_create(&th_klient, NULL, klientuj, &klient_data);

        pthread_join(th_klient, NULL);
        pthread_join(th_server,NULL);
        destroySockets(&server_data);
    }
    return 0;
}




