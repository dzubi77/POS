#ifndef MODERATOR_H
#define MODERATOR_H

#include <pthread.h>
#include "sockety/active_socket.h"
#include "sockety/char_buffer.h"

#define MAX_DLZKA_OTAZKY 256

typedef struct Moderator {
    int pocetHracov;
    int pocetOtazok;
    pthread_mutex_t* prihlasenieMutex;
    pthread_cond_t* otazkaPripravena;
    pthread_cond_t* odpovedPripravena;
    char aktualnaOtazka[MAX_DLZKA_OTAZKY];
    ACTIVE_SOCKET* moderator_sock;
} MODERATOR;

void* moderuj(void* data);

#undef MAX_DLZKA_OTAZKY
#endif //MODERATOR_H
