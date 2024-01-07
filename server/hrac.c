#include "hrac.h"

void* hraj(void* data) {
    HRAC_DATA* d = (HRAC_DATA*)data;
    //vlakno na pocuvanie
    pthread_t th_read;
    pthread_create(&th_read,NULL,pocuvaj, d);

    CHAR_BUFFER output;
    char_buffer_init(&output);

    while(d->a_sock->is_reading) {
        active_socket_try_get_read_data(d->a_sock, &output);
        if (output.size > 0) {
           //TODO zapisat odpoved do kvizu
        }
        usleep(10);
    }
    pthread_join(th_read,NULL);
    return NULL;
}

void* pocuvaj(void* data) {
    HRAC_DATA* d = (HRAC_DATA*)data;
    active_socket_start_reading(d->a_sock);
    return NULL;
}

void sendData(HRAC_DATA *d, char *message) {
    CHAR_BUFFER buf;
    char_buffer_init(&buf);
    char_buffer_append(&buf,message, strlen(message));
    active_socket_write_data(d->a_sock,&buf);
}

void skonci(HRAC_DATA *d) {
    active_socket_stop_reading(d->a_sock);
}