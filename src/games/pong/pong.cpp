#include "src/games/pong/pong.h"

// Dimensions of the game area
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// Symbols for the ball and paddles
#define BALL_CHAR (char)254
#define PADDLE_CHAR (char)219
#define CELL_SIZE 2

// Global variables
static int ballX = SCREEN_WIDTH / 2;
static int ballY = SCREEN_HEIGHT / 2;
static int ballVelocityX = 2;
static int ballVelocityY = 1;
static int playerPaddleY = SCREEN_HEIGHT / 2 - 4;
static int aiPaddleY = SCREEN_HEIGHT / 2 - 4;
static int playerScore = 0;
static int aiScore = 0;
static int winner = 0; // 0: No winner, 1: Player, 2: AI
static int aiDifficulty = -1; // 1: Easy, 2: Medium, 3: Hard

// Pong Difficulty Menu Options
MenuOption pongDifficultyOptions[3] = {
    {"Easy"},
    {"Medium"},
    {"Hard"}
};

// User input task handler
TaskHandle_t pongInputTaskHandle = NULL;
void TaskPongUserInput(void *pvParameters);

void playPong() {
    resetPong();
    selectPongDifficulty();

    // Start user input task
    xTaskCreate(TaskPongUserInput, "PongUserInput", 2048, NULL, 1, &pongInputTaskHandle);

    while (winner == 0) {
        // Update score
        if (ballX <= 5) {
            aiScore++;
            resetBall();
        } else if (ballX >= SCREEN_WIDTH - 5) {
            playerScore++;
            resetBall();
        }
        
        // Check ball bounces
        if (ballY <= 2 || ballY >= SCREEN_HEIGHT - 2) {
            ballVelocityY = -ballVelocityY; // Bounce off top/bottom
        }
        if ((ballX <= 7 && ballY >= playerPaddleY && ballY <= playerPaddleY + 8) || 
            (ballX >= SCREEN_WIDTH - 7 && ballY >= aiPaddleY && ballY <= aiPaddleY + 8)) {
            ballVelocityX = -ballVelocityX; // Bounce off paddles
        }

        // Move ball
        ballX += ballVelocityX;
        ballY += ballVelocityY;

        aiMove(ballY, ballX, aiPaddleY, aiDifficulty);

        // Update display
        updateDisplay(ballX, ballY, playerPaddleY, aiPaddleY, playerScore, aiScore);

        // Check for game over
        winner = (playerScore >= 5) ? 1 : (aiScore >= 5) ? 2 : 0;

        delay(20);
    }

    // Stop user input task
    if (pongInputTaskHandle != NULL) {
        vTaskDelete(pongInputTaskHandle);
        pongInputTaskHandle = NULL;
    }

    // Display winner
    clearScreen();
    if (winner == 1) {
        display.println("Player Wins!");
    } else {
        display.println("AI Wins!");
    }
    display.display();
    delay(3000);
}

void updateDisplay(int ballX, int ballY, int playerPaddleY, int aiPaddleY, int playerScore, int aiScore) {
    clearScreen();

    // Draw ball
    display.fillCircle(ballX, ballY, 2, SSD1306_WHITE);

    // Draw player paddle (left side)
    display.fillRect(2, playerPaddleY, 3, 8, SSD1306_WHITE);

    // Draw AI paddle (right side)
    display.fillRect(SCREEN_WIDTH - 5, aiPaddleY, 3, 8, SSD1306_WHITE);

    // Draw scores
    display.setTextSize(1);
    display.setCursor(SCREEN_WIDTH / 4, 0);
    display.print(String(playerScore));
    display.setCursor(3 * SCREEN_WIDTH / 4, 0);
    display.print(String(aiScore));

    display.display();
}

void aiMove(int ballY, int ballX, int &aiPaddleY, int aiDifficulty) {
    int reactionThreshold;
    int reactionSpeed;
    int activationDistance;
    
    switch (aiDifficulty) {
        case 1: // Easy
            reactionThreshold = 8;
            reactionSpeed = 3;
            activationDistance = 3 * SCREEN_WIDTH / 4;
            break;
        case 2: // Medium
            reactionThreshold = 5;
            reactionSpeed = 2;
            activationDistance = SCREEN_WIDTH / 2 + 20;
            break;
        case 3: // Hard
            reactionThreshold = 1;
            reactionSpeed = 1;
            activationDistance = SCREEN_WIDTH / 2;
            break;
        default:
            reactionThreshold = 8;
            reactionSpeed = 3;
            activationDistance = 3 * SCREEN_WIDTH / 4;
            break;
    }

    if (ballX >= activationDistance) { // Only move when ball is close enough
        int paddleCenter = aiPaddleY + 4;
        if (ballY < paddleCenter - reactionThreshold && aiPaddleY > 0) {
            aiPaddleY -= reactionSpeed;
            if (aiPaddleY < 0) aiPaddleY = 0;
        } else if (ballY > paddleCenter + reactionThreshold && aiPaddleY < SCREEN_HEIGHT - 8) {
            aiPaddleY += reactionSpeed;
            if (aiPaddleY > SCREEN_HEIGHT - 8) aiPaddleY = SCREEN_HEIGHT - 8;
        }
    }
}

void TaskPongUserInput(void *pvParameters) {
    while (true) {
        if (digitalRead(UP) == LOW && playerPaddleY > 0) {
            playerPaddleY--;
        } else if (digitalRead(DOWN) == LOW && playerPaddleY < SCREEN_HEIGHT - 8) {
            playerPaddleY++;
        }
        delay(20);
    }
}

void resetBall() {
    ballX = SCREEN_WIDTH / 2;
    ballY = SCREEN_HEIGHT / 2;
    ballVelocityX = (ballVelocityX > 0) ? -2 : 2; // Change direction
    ballVelocityY = (random(0, 2) == 0) ? -1 : 1; // Random vertical direction
}

void selectPongDifficulty() {
    int pongMenuSelectedOption = 0;
    while (aiDifficulty == -1) {
        clearScreen();

        if (digitalRead(UP) == LOW && pongMenuSelectedOption > 0) {
            pongMenuSelectedOption--;
            delay(200);
        } else if (digitalRead(DOWN) == LOW && pongMenuSelectedOption < 2) {
            pongMenuSelectedOption++;
            delay(200);
        } else if (digitalRead(RIGHT) == LOW) {
            aiDifficulty = (pongMenuSelectedOption + 1);
        }

        printMenu(pongDifficultyOptions, 3, "Select AI Difficulty", pongMenuSelectedOption);
        delay(200);
    }
}

void resetPong() {
    winner = 0;
    ballX = SCREEN_WIDTH / 2;
    ballY = SCREEN_HEIGHT / 2;
    ballVelocityX = 2;
    ballVelocityY = 1;
    playerPaddleY = SCREEN_HEIGHT / 2 - 4;
    aiPaddleY = SCREEN_HEIGHT / 2 - 4;
    playerScore = 0;
    aiScore = 0;
    aiDifficulty = -1;
}