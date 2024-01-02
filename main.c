#include <stdio.h>
#include "server/sockety/passive_socket.h"
#include "server/sockety/active_socket.h"
#include "klient/sockety/client_socket.h"

#include "server/moderator.h"
#include "klient/hrac.h"

int main(int argc, char** argv) {
    //server sockets init
    PASSIVE_SOCKET p_sock;
    ACTIVE_SOCKET a_sock;
    passive_socket_init(&p_sock);
    active_socket_init(&a_sock);
    passive_socket_start_listening(&p_sock, atoi(argv[2]));

    //sync mechanism init
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t otazkaPripravena = PTHREAD_COND_INITIALIZER;
    pthread_cond_t odpovedPripravena = PTHREAD_COND_INITIALIZER;

    //moderator init
    MODERATOR moderator = { 3, 5, &mutex, &otazkaPripravena, &odpovedPripravena };
    pthread_t th_moderator;
    pthread_create(&th_moderator, NULL, moderuj, &moderator);

    HRAC hraci[moderator.pocetHracov];
    pthread_t th_hrac[moderator.pocetHracov];
    for (int i = 0; i < moderator.pocetHracov; i++) {
        hraci[i].id = i + 1;
        pthread_create(&th_hrac[i], NULL, hraj, &hraci[i]);
    }

    CLIENT_SOCKET c_sock;
    clientInitialize(&c_sock, argv[1], argv[2]);
    clientConnectToServer(&c_sock);
    passive_socket_wait_for_client(&p_sock, &a_sock);

    clientCloseConnection(&c_sock);
    passive_socket_destroy(&p_sock);
    active_socket_destroy(&a_sock);

    return 0;
}
