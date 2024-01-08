//
// Created by petok on 8. 1. 2024.
//

#ifndef POS_SP_KLIENT_H
#define POS_SP_KLIENT_H

#include "sockety/client_socket.h"

typedef struct klient_data {
    char* port;
    int id;
    char* hostname;
    CLIENT_SOCKET* c_sock;
} KLIENT_DATA;

void* klientuj(void* data);
void parseMessage(const char *message);

#endif //POS_SP_KLIENT_H
