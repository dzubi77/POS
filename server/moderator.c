#include <pthread.h>
#include <stdio.h>
#include "moderator.h"
#include "kviz.h"

void* moderuj(void* data) {
    MODERATOR* d = (MODERATOR*) data;
    sendData(d->hrac1,"Vitajte v hre.");
    sendData(d->hrac2,"Vitajte v hre.");
//
//    //vytvorenie hracov
//    pthread_t th_hrac1, th_hrac2;
//    HRAC_DATA hrac_data1 = {1,d->hrac2->a_sock,d->quiz};
//    HRAC_DATA hrac_data2 = {2,d->hrac1->a_sock,d->quiz};
//    pthread_create(&th_hrac1,NULL, hraj, &hrac_data1);
//    pthread_create(&th_hrac2,NULL, hraj, &hrac_data2);
//
//    for (int i = 0; i < d->pocetOtazok; i++) {
//        pthread_mutex_lock(&d->quiz->prihlasenieMutex);
//        CHAR_BUFFER otazkaBuf;
//        char_buffer_init(&otazkaBuf);
//
//        //nacitanie otazky
//        int index = getRandomQuestion(&d->quiz);
//        char *otazka = d->quiz->question[index];
//        char_buffer_append(&otazkaBuf, otazka, sizeof(otazka));
//
//        //
//        d->quiz->moznaOdpoved = true;
//        pthread_cond_broadcast(&d->quiz->otazkaPripravena);
//        pthread_cond_wait(&d->quiz->odpovedPripravena, &d->quiz->prihlasenieMutex);
//
//        //vyhodnotenie odpovedi
//        d->quiz->moznaOdpoved = false;
//
//    }
    sendEndMessage(d->hrac1);
   sendEndMessage(d->hrac2);
//
//    pthread_join(th_hrac1, NULL);
//    pthread_join(th_hrac2,NULL);
    return NULL;
}