#include <stdio.h>
#include "server/sockety/passive_socket.h"
#include "server/sockety/active_socket.h"
#include "klient/sockety/client_socket.h"
#include "server/moderator.h"
#include "server/hrac.h"
#include "server/kviz.h"

#define MAX_CLIENTS 10


typedef struct server_data {
    short port;
    int pocet_hracov;
    int pocet_pripojenych;
    _Bool simulation;
    PASSIVE_SOCKET p_sock;
    HRAC_DATA* hrac1;
    HRAC_DATA *hrac2;
} SERVER_DATA;


typedef struct klient_data {
    char* port;
    char* hostname;
    CLIENT_SOCKET* c_sock;
} KLIENT_DATA;


void* serveruj(void* data) {
    SERVER_DATA *d = (SERVER_DATA *)data;
    QUIZ quiz;
    initializeQuiz(&quiz);
    passive_socket_init(&(d->p_sock));
    ACTIVE_SOCKET active_socket;
    active_socket_init(&active_socket);
    passive_socket_start_listening(&(d->p_sock), d->port);
    printf("Cakam na klienta.\n");
    passive_socket_wait_for_client(&d->p_sock,&active_socket);

    //vytvorenie prveho hraca
    d->hrac1 = &(HRAC_DATA){d->pocet_pripojenych, &active_socket, &quiz};
    d->pocet_pripojenych++;
    printf("server zaznamenal klienta.\n");

    if (d->simulation) {
        d->pocet_hracov = 2;
        printf("Caka sa na %d hracov.\n",d->pocet_hracov - d->pocet_pripojenych);
    } else {
        CHAR_BUFFER buf;
        char_buffer_init(&buf);
        char* message = "Kolko hracov na zacatie hry?.";
        char_buffer_append(&buf,message, strlen(message));
        active_socket_write_data(&active_socket,&buf);

        while(true) {

            usleep(10);
        }
    }
    while(d->pocet_hracov != d->pocet_pripojenych) {
        ACTIVE_SOCKET a_sock;
        active_socket_init(&a_sock);
        passive_socket_wait_for_client(&d->p_sock,&a_sock);

        //pridanie hraca
        d->hrac2 = &(HRAC_DATA){d->pocet_pripojenych, &a_sock, &quiz};
        d->pocet_pripojenych++;
        printf("AKtualne pripojenych %d hracov.\n", d->pocet_pripojenych);
    }
    passive_socket_stop_listening(&d->p_sock);
    passive_socket_destroy(&d->p_sock);

    //zacatie hry
    printf("Zaciatok hry.\n");
    MODERATOR moderator_data = {d->pocet_hracov, 5, d->hrac1, d->hrac2, &quiz};
    pthread_t moderator;
    pthread_create(&moderator,NULL, moderuj, &moderator_data);

    //ukoncenie hry
    pthread_join(moderator,NULL);
    destroyQuiz(&quiz);

    printf("Hra skoncena.\n");
    return NULL;
}

void* klientuj(void* data) {
    KLIENT_DATA *d = (KLIENT_DATA *)data;
    while (1) {
        char* output = (char*)malloc(100);
        memset(output, 0, 100);
        clientReceiveData(d->c_sock, output, 100);
        printf("received: %s.\n",output);
        if (strcmp(output, ":end") != 0) {
            clientSendData(d->c_sock,"a",100);
            free(output);
        } else {
            free(output);
            clientCloseConnection(d->c_sock);
            printf("Client disconnected.\n");
            return NULL;
        }
    }

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

    CLIENT_SOCKET c_sock2;
    KLIENT_DATA klient_data2;
    klient_data2.port = argv[2];
    klient_data2.hostname = argv[1];
    klient_data2.c_sock = &c_sock2;

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
        //ak sa podari rozbehat zmenit na false
        server_data.simulation = true;
        server_data.pocet_pripojenych = 0;

        pthread_t th_server;
        pthread_create(&th_server,NULL,serveruj,&server_data);
        sleep(2);
        //vytvorenie klienta
        printf("vytvara sa klient.\n");
        pthread_t th_klient;
        clientTryConnect(&klient_data);
        pthread_create(&th_klient, NULL, klientuj, &klient_data);

//vytvorenie klienta2
        printf("vytvara sa klient2.\n");
        pthread_t th_klient2;
        clientTryConnect(&klient_data2);
        pthread_create(&th_klient2, NULL, klientuj, &klient_data2);

        pthread_join(th_klient2, NULL);
        pthread_join(th_klient, NULL);
        pthread_join(th_server,NULL);
    }
    return 0;
}




