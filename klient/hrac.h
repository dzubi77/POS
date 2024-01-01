#ifndef POS_SP_HRAC_H
#define POS_SP_HRAC_H

#include <pthread.h>
#include <stdio.h>
#include "../kviz.h"
#include "sockety/client_socket.h"

typedef struct Hrac {
    int id;
    KVIZ* kviz;
    CLIENT_SOCKET* hrac_sock;
} HRAC;

void* hraj(void* data);

#endif //POS_SP_HRAC_H
