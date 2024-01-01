#ifndef POS_SP_KVIZ_H
#define POS_SP_KVIZ_H

#include <pthread.h>
#include <stdbool.h>
#define MAX_DLZKA_OTAZKY 256

typedef struct kviz {
    int pocetOtazok;
    int pocetHracov;
    pthread_mutex_t* prihlasenieMutex;
    pthread_cond_t* otazkaPripravena;
    pthread_cond_t* odpovedPripravena;
    char aktualnaOtazka[MAX_DLZKA_OTAZKY];
    _Bool moderatorZvoleny;
} KVIZ;

void kviz_init(KVIZ* k, int pPocetOtazok, int pPocetHracov, pthread_mutex_t* pMutex, pthread_cond_t* pQReady, pthread_cond_t* pAReady);
void kviz_destroy(KVIZ* k);

#undef MAX_DLZKA_OTAZKY
#endif //POS_SP_KVIZ_H
