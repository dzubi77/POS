#include <stdio.h>
#include "server/sockety/active_socket.h"
#include "klient/sockety/client_socket.h"

#include "server/moderator.h"
#include "klient/hrac.h"
#include "kviz.h"

int main(int argc, char** argv) {
    //init quiz
    KVIZ kviz;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t otazkaPripravena = PTHREAD_COND_INITIALIZER;
    pthread_cond_t odpovedPripravena = PTHREAD_COND_INITIALIZER;
    kviz_init(&kviz, 5, 3, &mutex, &otazkaPripravena, &odpovedPripravena);

    //init moderator thread
    ACTIVE_SOCKET a_sock;
    active_socket_init(&a_sock);

    MODERATOR moderator;
    moderator.kviz = &kviz;
    moderator.moderator_sock = &a_sock;
    pthread_t th_moderator;
    pthread_create(&th_moderator, NULL, moderuj, NULL);

    //init player threads
    HRAC hraci[kviz.pocetHracov];
    pthread_t th_hrac[kviz.pocetHracov];
    for (int i = 0; i < kviz.pocetHracov; i++) {
        hraci[i].id = i + 1;
        hraci[i].kviz = &kviz;
        CLIENT_SOCKET c_sock;
        clientInitialize(&c_sock, argv[1], argv[2]);
        hraci[i].hrac_sock = &c_sock;

        pthread_create(&th_hrac[i], NULL, hraj, NULL);
    }

    //waiting for threads and cleanup
    pthread_join(th_moderator, NULL);

    for (int i = 0; i < kviz.pocetHracov; i++) {
        clientCloseConnection(hraci[i].hrac_sock);
        pthread_join(th_hrac[i], NULL);
    }

    active_socket_destroy(&a_sock);
    kviz_destroy(&kviz);
}
