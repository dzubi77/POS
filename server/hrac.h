#ifndef POS_SP_HRAC_H
#define POS_SP_HRAC_H

#include <pthread.h>
#include <stdio.h>
#include "../klient/sockety/client_socket.h"
#include "sockety/active_socket.h"
#include "kviz.h"

typedef struct hrac_data {
    int id;
    ACTIVE_SOCKET* a_sock;
    _Bool hraSa;
    QUIZ *kviz;
} HRAC_DATA;

void* hraj(void* data);
void* pocuvaj(void* data);
void sendData(HRAC_DATA *d, const char *message);
void sendEndMessage(HRAC_DATA *d);
#endif //POS_SP_HRAC_H
