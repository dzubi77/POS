//
// Created by petok on 8. 1. 2024.
//
#include "server.h"
#include "moderator.h"

void* serveruj(void* data) {
    SERVER_DATA *d = (SERVER_DATA *)data;
    QUIZ quiz;
    initializeQuiz(&quiz);
    passive_socket_init(&(d->p_sock));
    ACTIVE_SOCKET active_socket;
    active_socket_init(&active_socket);
    passive_socket_start_listening(&(d->p_sock), d->port);
    printf("Cakam na klienta.\n");
    passive_socket_wait_for_client(&d->p_sock,&active_socket);

    //vytvorenie prveho hraca
    d->hrac1 = &(HRAC_DATA){d->pocet_pripojenych, &active_socket, &quiz};
    d->pocet_pripojenych++;
    printf("server zaznamenal klienta.\n");

    if (d->simulation) {
        d->pocet_hracov = 2;
        printf("Caka sa na %d hracov.\n",d->pocet_hracov - d->pocet_pripojenych);
    } else {
        CHAR_BUFFER buf;
        char_buffer_init(&buf);
        char* message = "Kolko hracov na zacatie hry?.";
        char_buffer_append(&buf,message, strlen(message));
        active_socket_write_data(&active_socket,&buf);

        while(true) {

            usleep(10);
        }
    }
    while(d->pocet_hracov != d->pocet_pripojenych) {
        ACTIVE_SOCKET a_sock;
        active_socket_init(&a_sock);
        passive_socket_wait_for_client(&d->p_sock,&a_sock);

        //pridanie hraca
        d->hrac2 = &(HRAC_DATA){d->pocet_pripojenych, &a_sock, &quiz};
        d->pocet_pripojenych++;
        printf("AKtualne pripojenych %d hracov.\n", d->pocet_pripojenych);
    }
    passive_socket_stop_listening(&d->p_sock);
    passive_socket_destroy(&d->p_sock);

    //zacatie hry
    MODERATOR moderator_data = {d->pocet_hracov, 2, d->hrac1, d->hrac2, &quiz};
    pthread_t moderator;
    pthread_create(&moderator,NULL, moderuj, &moderator_data);

    //ukoncenie hry
    pthread_join(moderator,NULL);
    destroyQuiz(&quiz);
    return NULL;
}
