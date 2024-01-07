#include <pthread.h>
#include <stdio.h>
#include "moderator.h"
#include "kviz.h"

void* moderuj(void* data) {
    MODERATOR* d = (MODERATOR*) data;
    for(int i = 0; i < d->pocetHracov; i++){
        sendData(&d->hraci[i],"Vitajte v hre.");
    }
    for (int i = 0; i < d->pocetOtazok; i++) {
        pthread_mutex_lock(&d->quiz->prihlasenieMutex);
        CHAR_BUFFER otazkaBuf;
        char_buffer_init(&otazkaBuf);

        //nacitanie otazky
        int index = getRandomQuestion(&d->quiz);
        char *otazka = d->quiz->question[index];
        char_buffer_append(&otazkaBuf, otazka, sizeof(otazka));

        pthread_cond_broadcast(&d->quiz->otazkaPripravena);
        pthread_mutex_unlock(&d->quiz->prihlasenieMutex);

        while (!&d->quiz->odpovedPripravena) {
            pthread_cond_wait(&d->quiz->odpovedPripravena, &d->quiz->prihlasenieMutex);
        }
    }

    return NULL;
}