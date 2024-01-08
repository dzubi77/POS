#include "hrac.h"

void* hraj(void* data) {
    HRAC_DATA* d = (HRAC_DATA*)data;
    //vlakno na pocuvanie
    pthread_t th_read;
    pthread_create(&th_read,NULL,pocuvaj, d);

    CHAR_BUFFER output;
    char_buffer_init(&output);
    if(d->hraSa && d->kviz->hraSa) {
        for (int i = 0; i < 5; i++) {
            pthread_mutex_lock(&d->kviz->prihlasenieMutex);
            if (d->kviz->moznaOdpoved) {
                pthread_cond_wait(&d->kviz->otazkaPripravena, &d->kviz->prihlasenieMutex);
                pthread_mutex_unlock(&d->kviz->prihlasenieMutex);
                usleep(rand() % 6000 + 1000);
                if (pthread_mutex_trylock(&d->kviz->prihlasenieMutex) != 0) {
                    pthread_cond_wait(&d->kviz->mutexVolny, &d->kviz->prihlasenieMutex);
                }
                int odpoved = rand() % 3 + 1;
                if (d->id == 1) {
                    d->kviz->odpoved1 = odpoved;

                } else {
                    d->kviz->odpoved2 = odpoved;
                }
                if (d->kviz->prvaOdpoved == 0) {
                    d->kviz->prvaOdpoved = d->id;
                }

                if (d->kviz->odpoved1 != 0 && d->kviz->odpoved2 != 0) {
                    pthread_cond_signal(&d->kviz->odpovedPripravena);
                    pthread_mutex_unlock(&d->kviz->prihlasenieMutex);
                } else {
                    pthread_cond_signal(&d->kviz->mutexVolny);
                    pthread_mutex_unlock(&d->kviz->prihlasenieMutex);
                }
            } else {
                pthread_mutex_unlock(&d->kviz->prihlasenieMutex);
            }
        }
    }
    pthread_join(th_read,NULL);
    char_buffer_destroy(&output);
    return NULL;
}

void* pocuvaj(void* data) {
    HRAC_DATA* d = (HRAC_DATA*)data;
    active_socket_start_reading(d->a_sock);
    return NULL;
}

void sendData(HRAC_DATA *d, const char *message) {
    CHAR_BUFFER buf;
    char_buffer_init(&buf);
    char_buffer_append(&buf,message, strlen(message));
    active_socket_write_data(d->a_sock,&buf);
}

void sendEndMessage(HRAC_DATA *d) {
    d->hraSa = false;
    active_socket_write_end_message(d->a_sock);
    active_socket_stop_reading(d->a_sock);
}
