#ifndef POS_SP_CLIENT_SOCKET_H
#define POS_SP_CLIENT_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct client_socket {
    int sock_fd;
    struct sockaddr_in serv_addr;
    struct hostent* server;
} CLIENT_SOCKET;

void error(const char* message);
int clientInitialize(CLIENT_SOCKET* client, const char* hostname, const char* port);
int clientConnectToServer(CLIENT_SOCKET* client);
void clientSendData(CLIENT_SOCKET* client, const char* message);
void clientReceiveData(CLIENT_SOCKET* client, char* buffer, int bufferSize);
void clientCloseConnection(CLIENT_SOCKET* client);
_Bool clientIsConnected(CLIENT_SOCKET* client);

#endif //POS_SP_CLIENT_SOCKET_H
