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
    //passive_socket_stop_listening(&(data->p_sock));
    active_socket_destroy(&(data->a_sock));
    //passive_socket_destroy(&(data->p_sock));
}

typedef struct klient_data {
    char* port;
    char* hostname;
    CLIENT_SOCKET* c_sock;
} KLIENT_DATA;

void* consume(void* data) {
    SERVER_DATA *d = (SERVER_DATA *)data;
    CHAR_BUFFER output;
    char_buffer_init(&output);
    printf("snazim sa citat.\n");
    int i = 0;
    while(d->a_sock.is_reading == false){
        sleep(2);
    }
    while(d->a_sock.is_reading) {
        printf("citam,output: %s.\n", output.data);
        active_socket_try_get_read_data(&d->a_sock, &output);
        if (output.size > 0) {
            printf("nieco tam je.\n");
            printf(output.data);
            active_socket_stop_reading(&d->a_sock);
            printf("malo by sa zastavit\n");
        }
        if (i == 1000){
            active_socket_stop_reading(&d->a_sock);
            printf("stop po 1000 opakovaniach\n");
            break;
        }
        i++;
        usleep(10);
    }
    printf("docital som.\n");

}

void* serveruj(void* data) {
    SERVER_DATA *d = (SERVER_DATA *)data;
    passive_socket_wait_for_client(&d->p_sock,&d->a_sock);
    passive_socket_stop_listening(&d->p_sock);
    passive_socket_destroy(&d->p_sock);
    printf("server zaznamenal klienta.\n");
    struct char_buffer buffer;
    char_buffer_init(&buffer);
    char message[] = "sprava pre klienta";
    char_buffer_append(&buffer, message, sizeof(message) - 1);

    active_socket_write_data(&d->a_sock, &buffer);
    active_socket_start_reading(&d->a_sock);
    //sync mechanism init
//    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//    pthread_cond_t otazkaPripravena = PTHREAD_COND_INITIALIZER;
//    pthread_cond_t odpovedPripravena = PTHREAD_COND_INITIALIZER;

    //moderator init
//    MODERATOR moderator = { 0, 5, &mutex, &otazkaPripravena, &odpovedPripravena };
//    pthread_t th_moderator;
//    pthread_create(&th_moderator, NULL, moderuj, &moderator);


}

void* klientuj(void* data) {
    KLIENT_DATA *d = (KLIENT_DATA *)data;
    char* output = (char*)malloc(100);
    clientReceiveData(d->c_sock,output,100);
    printf("output: %s\n", output);
    clientSendData(d->c_sock,"sprava pre server");
    printf("klient poslal spravu.\n");
    //printf("zadaj nieco\n");
//    char input[100];
//    scanf("%s", input);
    //clientSendData(d->c_sock,input);
    sleep(5);
    clientCloseConnection(&d->c_sock);
    printf("Client disconnected.\n");

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
        pthread_t th_server, th_consume;
        pthread_create(&th_server,NULL,serveruj,&server_data);
        sleep(1);

        //vytvorenie klienta
        printf("vytvara sa klient.\n");
        pthread_t th_klient;
        clientTryConnect(&klient_data);
        pthread_create(&th_klient, NULL, klientuj, &klient_data);

        pthread_create(&th_consume,NULL,consume, &server_data);

        pthread_join(th_klient, NULL);
        pthread_join(th_server,NULL);
        pthread_join(th_consume,NULL);
        destroySockets(&server_data);
    }
    return 0;
}




