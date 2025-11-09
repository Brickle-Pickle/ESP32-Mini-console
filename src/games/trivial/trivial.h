#ifndef TRIVIAL_H
#define TRIVIAL_H

#include "src/os/utils.h"

void playTrivial();
void getTrivialQuestions();
void renderTrivialQuestion(String line, int questionIndex);
void displayTrivialQuestion(TrivialQuestion question, int maxQuestions, int selectedOption);
int getRandomTrivialQuestion();
void checkTrivialAnswer(int selectedAnswer);
void setupTrivialData();

#endif