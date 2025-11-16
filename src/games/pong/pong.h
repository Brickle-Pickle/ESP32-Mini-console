#ifndef PONG_H
#define PONG_H

#include "src/os/utils.h"

void playPong();
void resetPong();
void selectPongDifficulty();
void resetBall();
void TaskPongUserInput(void *pvParameters);
void aiMove(int ballY, int ballX, int &aiPaddleY, int aiDifficulty);
void updateDisplay(int ballX, int ballY, int playerPaddleY, int aiPaddleY, int playerScore, int aiScore);

#endif 