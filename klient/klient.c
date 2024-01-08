//
// Created by petok on 8. 1. 2024.
//

#include <stdio.h>
#include <string.h>
#include "klient.h"

void parseMessage(const char *message) {
    char indikator = message[0];

    if (indikator == 'Q') {
        char *token;
        char indexOtazky = message[1];
        char *rest = (char *)(message+2);
        token = strtok_r(rest, ";", &rest);
        printf("Otazka %c: %s\n", indexOtazky,token);
        int index = 1;
        while ((token = strtok_r(rest, ";", &rest)) != NULL) {
            printf("%d: %s ", index, token);
            index++;
        }
        printf("\n");

    } else {
        printf("%s\n",message);
    }
}

void* klientuj(void* data) {
    KLIENT_DATA *d = (KLIENT_DATA *)data;
    while (1) {
        char* output = (char*)malloc(100);
        memset(output, 0, 100);
        clientReceiveData(d->c_sock, output, 100);
        if (strcmp(output, ":end") != 0) {
            printf("Klient %d:\n",d->id);
            parseMessage(output);
            clientSendData(d->c_sock,"a",100);
            free(output);
        } else {
            free(output);
            clientCloseConnection(d->c_sock);
            printf("Klient %d sa odhlasil.\n",d->id);
            return NULL;
        }
    }
}