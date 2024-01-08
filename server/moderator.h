#ifndef MODERATOR_H
#define MODERATOR_H

#include <pthread.h>
#include "sockety/active_socket.h"
#include "sockety/char_buffer.h"
#include "hrac.h"

typedef struct Moderator {
    int pocetHracov;
    int pocetOtazok;
    HRAC_DATA *hrac1;
    HRAC_DATA *hrac2;
    QUIZ *quiz;
} MODERATOR;

void* moderuj(void* data);
int sendQuestion(MODERATOR *d, int cisloOtazky);
#endif //MODERATOR_H
