#ifndef MODERATOR_H
#define MODERATOR_H

#include "../kviz.h"
#include "sockety/active_socket.h"
#include "sockety/char_buffer.h"

typedef struct Moderator {
    KVIZ* kviz;
    ACTIVE_SOCKET* moderator_sock;
} MODERATOR;

void* moderuj(void* data);

#endif //MODERATOR_H
