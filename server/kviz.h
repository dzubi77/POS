//
// Created by petok on 6. 1. 2024.
//

#ifndef POS_SP_KVIZ_H
#define POS_SP_KVIZ_H

#include <malloc.h>
#include <stdlib.h>
#define NUM_QUESTIONS 6
#define NUM_ANSWERS 3

typedef struct quiz {
    int count;
    char **question;
    char *answers[NUM_QUESTIONS][NUM_ANSWERS];
    int *correctAnswerIndex;
    int* status;
} QUIZ;

void initializeQuiz(QUIZ* quiz) {
    quiz->question = (char*[]){"Aké je hlavné mesto Slovenska?",
                               "Ktorá rieka preteká cez Bratislavu?",
                               "Koľko milionov obyvateľov má Slovensko?",
                               "Ktorý slovenský hrad je najväčší?",
                               "Kto je zakladateľom prvej slovenskej republiky?",
                               "Aké je tradičné slovenské jedlo?"};
    quiz->correctAnswerIndex = (int[]){2,0,1,0,2,0};
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
    quiz->answers[4][2] = "Milan Rastislav Štefánik";

    quiz->answers[5][0] = "Bryndzové halušky";
    quiz->answers[5][1] = "Knedle";
    quiz->answers[5][2] = "Pirohy";

    quiz->status = (int[]){0,0,0,0,0,0};
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

#endif //POS_SP_KVIZ_H
