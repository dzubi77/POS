//
// Created by petok on 8. 1. 2024.
//

#ifndef POS_SP_SERVER_H
#define POS_SP_SERVER_H

#include "hrac.h"
#include "sockety/passive_socket.h"

typedef struct server_data {
    short port;
    int pocet_hracov;
    int pocet_pripojenych;
    _Bool simulation;
    PASSIVE_SOCKET p_sock;
    HRAC_DATA* hrac1;
    HRAC_DATA *hrac2;
} SERVER_DATA;

void* serveruj(void* data);

#endif //POS_SP_SERVER_H
