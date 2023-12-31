//
// Created by petok on 6. 1. 2024.
//

#ifndef POS_SP_KVIZ_H
#define POS_SP_KVIZ_H

#include <malloc.h>
#include <stdlib.h>
#define NUM_QUESTIONS 10
#define NUM_ANSWERS 3

typedef struct quiz {
    int count;
    char **question;
    char *answers[NUM_QUESTIONS][NUM_ANSWERS];
    int correctAnswerIndex[NUM_QUESTIONS];
    int odpoved1;
    int odpoved2;
    _Bool moznaOdpoved;
    _Bool hraSa;
    int prvaOdpoved;
    int status[NUM_QUESTIONS];
    pthread_mutex_t prihlasenieMutex;
    pthread_cond_t otazkaPripravena;
    pthread_cond_t mutexVolny;
    pthread_cond_t odpovedPripravena;
} QUIZ;
void initializeQuiz(QUIZ* quiz);
int getRandomQuestion(QUIZ* quiz);
void destroyQuiz(QUIZ *quiz);
#endif //POS_SP_KVIZ_H
