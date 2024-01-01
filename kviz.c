#include "kviz.h"

void kviz_init(KVIZ* k, int pPocetOtazok, int pPocetHracov, pthread_mutex_t* pMutex, pthread_cond_t* pQReady, pthread_cond_t* pAReady) {
    k->pocetOtazok = pPocetOtazok;
    k->pocetHracov = pPocetHracov;
    k->moderatorZvoleny = false;
    k->prihlasenieMutex = pMutex;
    k->otazkaPripravena = pQReady;
    k->odpovedPripravena = pAReady;
}

void kviz_destroy(KVIZ* k) {
    k->pocetOtazok = 0;
    k->pocetHracov = 0;
    k->moderatorZvoleny = false;
    pthread_mutex_destroy(k->prihlasenieMutex);
    pthread_cond_destroy(k->otazkaPripravena);
    pthread_cond_destroy(k->odpovedPripravena);
}