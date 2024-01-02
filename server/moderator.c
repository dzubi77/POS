#include <pthread.h>
#include <stdio.h>
#include "moderator.h"

void* moderuj(void* data) {
    MODERATOR* d = (MODERATOR*) data;

    for (int i = 0; i < d->pocetOtazok; i++) {
        pthread_mutex_lock(d->prihlasenieMutex);
        CHAR_BUFFER otazkaBuf;
        char_buffer_init(&otazkaBuf);

        char otazka[256];
        //nacitanie otazky

        char_buffer_append(&otazkaBuf, otazka, sizeof(otazka));
        active_socket_write_data(d->moderator_sock, &otazkaBuf);
        pthread_cond_broadcast(d->otazkaPripravena);
        pthread_mutex_unlock(d->prihlasenieMutex);

        while (!d->odpovedPripravena) {
            pthread_cond_wait(d->odpovedPripravena, d->prihlasenieMutex);
        }
    }

    return NULL;
}