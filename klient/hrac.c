#include "hrac.h"

void* hraj(void* data) {
    HRAC* d = (HRAC*)data;
    for (int i = 0; i < d->pocetOtazok; i++) {
        pthread_mutex_lock(&d->mutexPrihlasenie);
        //otazka neni pripravena
        while (i) {
            pthread_cond_wait(&d->condOtazka, &d->mutexPrihlasenie);
        }

        // TODO: algoritmus vytvorenia odpovede

        //zodpovedane, posleme signal
        pthread_cond_signal(&d->condOdhlasovane);
        pthread_mutex_unlock(&d->mutexPrihlasenie);
    }
    return NULL;
}