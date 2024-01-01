#include <pthread.h>
#include <stdio.h>
#include "moderator.h"

void* moderuj(void* data) {
    MODERATOR* d = (MODERATOR*) data;

    for (int i = 0; i < d->kviz->pocetOtazok; i++) {
        pthread_mutex_lock(d->kviz->prihlasenieMutex);
        CHAR_BUFFER otazkaBuf;
        char_buffer_init(&otazkaBuf);

        char otazka[256];
        //nacitanie otazky

        char_buffer_append(&otazkaBuf, otazka, sizeof(otazka));
        active_socket_write_data(d->moderator_sock, &otazkaBuf);
        pthread_cond_broadcast(d->kviz->otazkaPripravena);
        pthread_mutex_unlock(d->kviz->prihlasenieMutex);

        while (!d->kviz->odpovedPripravena) {
            pthread_cond_wait(d->kviz->odpovedPripravena, d->kviz->prihlasenieMutex);
        }
    }

    return NULL;
}