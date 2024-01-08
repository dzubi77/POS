#include <pthread.h>
#include <stdio.h>
#include "moderator.h"
#include "kviz.h"
#include "sockety/char_buffer.h"
#include "sockety/linked_list.h"

void* moderuj(void* data) {
    MODERATOR* d = (MODERATOR*) data;
    sendData(d->hrac1,"Vitajte v hre.");
    sendData(d->hrac2,"Vitajte v hre.");
    d->quiz->hraSa = true;
    d->quiz->moznaOdpoved = true;
    //vytvorenie hracov
    pthread_t th_hrac1, th_hrac2;
    HRAC_DATA hrac_data2 = {1,d->hrac2->a_sock,true, d->quiz};
    HRAC_DATA hrac_data1 = {2,d->hrac1->a_sock,true, d->quiz};
    pthread_create(&th_hrac1,NULL, hraj, &hrac_data1);
    pthread_create(&th_hrac2,NULL, hraj, &hrac_data2);

    sleep(1);
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&d->quiz->prihlasenieMutex);
        CHAR_BUFFER buf;
        char_buffer_init(&buf);

        //poslanie otazky
        int index = getRandomQuestion(d->quiz);
        const char *otazka = d->quiz->question[index];
        const char *odpoved1 = d->quiz->answers[index][0];
        const char *odpoved2 = d->quiz->answers[index][1];
        const char *odpoved3 = d->quiz->answers[index][2];
        char indikator = 'Q';
        char_buffer_append(&buf, &indikator, 1);
        char buffer[10];
        snprintf(buffer, 10, "%d", (i+1));
        char_buffer_append(&buf, buffer, strlen(buffer));

        char_buffer_append(&buf, otazka, strlen(otazka));
        char_buffer_append(&buf, ";", 1);
        char_buffer_append(&buf, odpoved1, strlen(odpoved1));
        char_buffer_append(&buf, ";", 1);
        char_buffer_append(&buf, odpoved2, strlen(odpoved2));
        char_buffer_append(&buf, ";", 1);
        char_buffer_append(&buf, odpoved3, strlen(odpoved3));
        active_socket_write_data(d->hrac1->a_sock, &buf);
        active_socket_write_data(d->hrac2->a_sock, &buf);
        char_buffer_destroy(&buf);
        pthread_cond_broadcast(&d->quiz->otazkaPripravena);
        pthread_mutex_unlock(&d->quiz->prihlasenieMutex);
        //cakam na odpovede
        pthread_cond_wait(&d->quiz->odpovedPripravena, &d->quiz->prihlasenieMutex);
        //vsetci odpovedali vyhodnotenie odpovedi

        //priprava na dalsie kolo
        d->quiz->odpoved1 = 0;
        d->quiz->odpoved2 = 0;
        d->quiz->prvaOdpoved = 0;
        pthread_mutex_unlock(&d->quiz->prihlasenieMutex);
        sleep(5);
    }
    d->quiz->hraSa = false;
    sleep(1);
    sendEndMessage(d->hrac1);
    sendEndMessage(d->hrac2);
    pthread_join(th_hrac1, NULL);
    pthread_join(th_hrac2,NULL);
    printf("ukonceny moderator. ");
    return NULL;
}