//
// Created by petok on 7. 1. 2024.
//
#include <pthread.h>
#include <stdbool.h>
#include "kviz.h"

void initializeQuiz(QUIZ* quiz) {
    quiz->count = 10;
    quiz->hraSa = false;
    quiz->question = malloc(sizeof(char*) * quiz->count);
    quiz->question[0] = "Aké je hlavné mesto Slovenska?";
    quiz->question[1] = "Ktorá rieka preteká cez Bratislavu?";
    quiz->question[2] = "Koľko milionov obyvateľov má Slovensko?";
    quiz->question[3] = "Ktorý slovenský hrad je najväčší?";
    quiz->question[4] = "Kto je zakladateľom prvej slovenskej republiky?";
    quiz->question[5] = "Aké je tradičné slovenské jedlo?";
    quiz->question[6] = "V ktorom roku vzniklo Slovensko?";
    quiz->question[7] = "Ktorá hora je najvyššia na Slovensku?";
    quiz->question[8] = "Kto je autorom hymny Slovenskej republiky?";
    quiz->question[9] = "Koľko korún je najväčšou bankovkou v dejinách Slovenska?";
    int correctAnswers[] = {2, 0, 1, 0, 2, 0, 1, 0, 2, 1};
    for (int i = 0; i < NUM_QUESTIONS; ++i) {
        quiz->correctAnswerIndex[i] = correctAnswers[i];
    }
    quiz->answers[0][0] = "Brno";
    quiz->answers[0][1] = "Budapešť";
    quiz->answers[0][2] = "Bratislava";

    quiz->answers[1][0] = "Dunaj";
    quiz->answers[1][1] = "Váh";
    quiz->answers[1][2] = "Hron";

    quiz->answers[2][0] = "3.9";
    quiz->answers[2][1] = "5.4";
    quiz->answers[2][2] = "6.2";

    quiz->answers[3][0] = "Spišský hrad";
    quiz->answers[3][1] = "Devín";
    quiz->answers[3][2] = "Trenčiansky hrad";

    quiz->answers[4][0] = "Jozef Tiso";
    quiz->answers[4][1] = "Andrej Hlinka";
    quiz->answers[4][2] = "M.R. Štefánik";

    quiz->answers[5][0] = "Bryndzové halušky";
    quiz->answers[5][1] = "Knedle";
    quiz->answers[5][2] = "Pirohy";

    quiz->answers[6][0] = "1993";
    quiz->answers[6][1] = "2000";
    quiz->answers[6][2] = "1989";

    quiz->answers[7][0] = "Gerlachovský štít";
    quiz->answers[7][1] = "Kriváň";
    quiz->answers[7][2] = "Rysy";

    quiz->answers[8][0] = "Andrej Sládkovič";
    quiz->answers[8][1] = "Janko Matúška";
    quiz->answers[8][2] = "Ján Kollár";

    quiz->answers[9][0] = "1,000,000";
    quiz->answers[9][1] = "100,000";
    quiz->answers[9][2] = "1,000";

    quiz->moznaOdpoved = false;
    int statusy[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < NUM_QUESTIONS; ++i) {
        quiz->status[i] = statusy[i];
    }
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    quiz->prihlasenieMutex = mutex;
    pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
    quiz->odpovedPripravena = cond1;
    pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
    quiz->otazkaPripravena = cond2;
    pthread_cond_t cond3 = PTHREAD_COND_INITIALIZER;
    quiz->mutexVolny = cond3;
}

int getRandomQuestion(QUIZ* quiz) {
    int unansweredQuestions = 0;
    for (int i = 0; i < quiz->count; ++i) {
        if (quiz->status[i] == 0) {
            unansweredQuestions++;
        }
    }

    if (unansweredQuestions == 0) {
        return -1;
    }

    int randomIndex;
    do {
        randomIndex = rand() % quiz->count;
    } while (quiz->status[randomIndex] == 1);

    quiz->status[randomIndex] = 1;  // Mark the question as sent
    return randomIndex;
}
void destroyQuiz(QUIZ *quiz) {
    free(quiz->question);
    pthread_mutex_destroy(&quiz->prihlasenieMutex);
    pthread_cond_destroy(&quiz->otazkaPripravena);
    pthread_cond_destroy(&quiz->odpovedPripravena);
    pthread_cond_destroy(&quiz->mutexVolny);
}