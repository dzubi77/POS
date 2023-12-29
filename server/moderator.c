//
// Created by petok on 14. 12. 2023.
//
#include <pthread.h>
#include <stdio.h>
#include "moderator.h"


void* moderuj(void* data) {
    MODERATOR* d = (MODERATOR*) data;
    printf("Zacina sa kviz. Dnes tu mame %d hracov. \n", d->pocetHracov);
    printf("Odpovedat budu na %d otazok. \n", d->pocetOtazok);
    for (int i = 0; i < d->pocetOtazok; i++) {
        pthread_mutex_lock(&d->mutexPrihlasenie);

        // TODO: vygenerovanie otazky

        pthread_mutex_unlock(&d->mutexPrihlasenie);
    }
    return NULL;
}