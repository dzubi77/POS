#include "client_socket.h"
#define INVALID_SOCKET -1

void error(const char* message) {
    perror(message);
}

int clientInitialize(CLIENT_SOCKET* client, const char* hostname, const char* port) {
    client->server = gethostbyname(hostname);
    if (client->server == NULL) {
        printf("No such host!");
        return 1;
    }

    bzero((char*)&client->serv_addr, sizeof(client->serv_addr));
    client->serv_addr.sin_family = AF_INET;
    bcopy((char*)client->server->h_addr, (char*)&client->serv_addr.sin_addr.s_addr, client->server->h_length);
    client->serv_addr.sin_port = htons(atoi(port));

    client->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client->sock_fd < 0) {
        error("Error while creating socket!");
        return 1;
    }
    return 0;
}

int clientConnectToServer(CLIENT_SOCKET* client) {
    if (connect(client->sock_fd, (struct sockaddr*)&client->serv_addr, sizeof(client->serv_addr)) < 0) {
        error("Error while connecting to socket!");
        return 1;
    }
    return 0;
}

void clientSendData(CLIENT_SOCKET* client, const char* message) {
    int n = write(client->sock_fd, message, strlen(message));
    if (n < 0) {
        error("Error while writing to socket!");
    }
}

void clientReceiveData(CLIENT_SOCKET* client, char* buffer, int bufferSize) {
    int n = read(client->sock_fd, buffer, bufferSize - 1);
    if (n < 0) {
        error("Error reading from socket");
    }
    printf("%s \n", buffer);
}

void clientCloseConnection(CLIENT_SOCKET* client) {
    close(client->sock_fd);
}

_Bool clientIsConnected(CLIENT_SOCKET* client) {
    if (client->sock_fd >= 0) {
        printf("Client connected \n");
        return true;
    } else {
        printf("Not connected \n");
        return false;
    }
}

#undef INVALID_SOCKET