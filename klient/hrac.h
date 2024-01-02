#ifndef POS_SP_HRAC_H
#define POS_SP_HRAC_H

#include <pthread.h>
#include <stdio.h>
#include "sockety/client_socket.h"

#define MAX_DLZKA_ODPOVED 256

typedef struct Hrac {
    int id;
    CLIENT_SOCKET* hrac_sock;
    char odpoved[MAX_DLZKA_ODPOVED];
} HRAC;

void* hraj(void* data);

#undef MAX_DLZKA_ODPOVED
#endif //POS_SP_HRAC_H
