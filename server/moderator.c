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
    int bodyHrac1 = 0;
    int bodyHrac2 = 0;
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&d->quiz->prihlasenieMutex);
        int questionIndex = sendQuestion(d, i+1);

        pthread_cond_broadcast(&d->quiz->otazkaPripravena);
        pthread_mutex_unlock(&d->quiz->prihlasenieMutex);
        //cakam na odpovede
        pthread_cond_wait(&d->quiz->odpovedPripravena, &d->quiz->prihlasenieMutex);
        //vsetci odpovedali vyhodnotenie odpovedi
        int spravnaOdpoved = d->quiz->correctAnswerIndex[questionIndex];
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Prvý bol hráč číslo %d\nHráč 1 odpovedal %d. Hráč 2 odpovedal %d.\nSprávna odpoveď bola %d.", d->quiz->prvaOdpoved,d->quiz->odpoved1,d->quiz->odpoved2, spravnaOdpoved+1);
        sendData(d->hrac1, buffer);
        sendData(d->hrac2, buffer);

        if (d->quiz->prvaOdpoved == 1)
        {
            if ((d->quiz->odpoved1-1) == spravnaOdpoved) {
                bodyHrac1 += 2;
                if ((d->quiz->odpoved2 - 1) == spravnaOdpoved) {
                    bodyHrac2 += 1;
                }
            } else {
                if ((d->quiz->odpoved2 - 1) == spravnaOdpoved) {
                    bodyHrac2 += 2;
                }
            }
        } else {
            if ((d->quiz->odpoved2-1) == spravnaOdpoved) {
                bodyHrac2 += 2;
                if ((d->quiz->odpoved1 - 1) == spravnaOdpoved) {
                    bodyHrac1 += 1;
                }
            } else {
                if ((d->quiz->odpoved1 - 1) == spravnaOdpoved) {
                    bodyHrac1 += 2;
                }
            }
        }

        snprintf(buffer, sizeof(buffer), "Body: hráč 1: %d, hráč 2: %d", bodyHrac1, bodyHrac2);
        sendData(d->hrac1, buffer);
        sendData(d->hrac2, buffer);
        //priprava na dalsie kolo
        d->quiz->odpoved1 = 0;
        d->quiz->odpoved2 = 0;
        d->quiz->prvaOdpoved = 0;
        pthread_mutex_unlock(&d->quiz->prihlasenieMutex);

        sleep(5);
        printf("-----------------------------------------------------\n");
    }

    d->quiz->hraSa = false;
    //vyhodnotenie kvizu
    char buffer[100];
    if (bodyHrac1 == bodyHrac2){
        snprintf(buffer, sizeof(buffer), "Bola to remíza.");
        sendData(d->hrac1, buffer);
        sendData(d->hrac2, buffer);
    } else {
        int vyherca = bodyHrac2 > bodyHrac1 ? 2 : 1;
        snprintf(buffer, sizeof(buffer), "Vyhral hráč číslo %d.", vyherca);
        sendData(d->hrac1, buffer);
        sendData(d->hrac2, buffer);
    }
    snprintf(buffer, sizeof(buffer), "Dovidenia v ďalšej hre.\n");
    sendData(d->hrac1, buffer);
    sendData(d->hrac2, buffer);

    sleep(1);
    sendEndMessage(d->hrac1);
    sendEndMessage(d->hrac2);
    pthread_join(th_hrac1, NULL);
    pthread_join(th_hrac2,NULL);

    return NULL;
}

int sendQuestion(MODERATOR *d, int cisloOtazky) {
    //poslanie otazky
    CHAR_BUFFER buf;
    char_buffer_init(&buf);
    int index = getRandomQuestion(d->quiz);
    const char *otazka = d->quiz->question[index];
    const char *odpoved1 = d->quiz->answers[index][0];
    const char *odpoved2 = d->quiz->answers[index][1];
    const char *odpoved3 = d->quiz->answers[index][2];
    char indikator = 'Q';
    char_buffer_append(&buf, &indikator, 1);
    char buffer[10];
    snprintf(buffer, 10, "%d", cisloOtazky);
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
    return index;
}