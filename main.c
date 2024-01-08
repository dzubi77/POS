#include <stdio.h>
#include "server/sockety/active_socket.h"
#include "klient/sockety/client_socket.h"
#include "server/kviz.h"
#include "klient/klient.h"
#include "server/server.h"

int clientTryConnect(KLIENT_DATA* data) {
    if (clientInitialize(data->c_sock, data->hostname, data->port) == 0 && clientConnectToServer(data->c_sock) == 0) {
        return 0;
    }
    return 1;
}

int main(int argc, char** argv) {

    CLIENT_SOCKET c_sock;
    KLIENT_DATA klient_data;
    klient_data.port = argv[2];
    klient_data.hostname = argv[1];
    klient_data.c_sock = &c_sock;
    klient_data.id = 1;

    CLIENT_SOCKET c_sock2;
    KLIENT_DATA klient_data2;
    klient_data2.port = argv[2];
    klient_data2.hostname = argv[1];
    klient_data2.c_sock = &c_sock2;
    klient_data2.id = 2;
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
        sleep(1);

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




