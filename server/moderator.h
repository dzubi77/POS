//
// Created by petok on 14. 12. 2023.
//

#ifndef MODERATOR_H
#define MODERATOR_H
#include <pthread.h>

typedef struct Moderator {
    int pocetOtazok;
    int pocetHracov;
    pthread_mutex_t mutexPrihlasenie;
    pthread_cond_t condOtazka;
    pthread_cond_t condOdhlasovane;
} MODERATOR;

void* moderuj(void* data);

#endif //MODERATOR_H
