#include "src/games/snake/snake.h"
#include <FS.h>
#include <SPIFFS.h>

// Dimensions of the game area
#define GRID_WIDTH 16
#define GRID_HEIGHT 4

// Symbols for the snake and the apple
#define SNAKE_CHAR (char)254
#define APPLE_CHAR (char)248
#define CELL_SIZE 8

// Global variables
int snakeScore = 0;
int snakeLength = 1;
int snakeX[GRID_WIDTH * GRID_HEIGHT];
int snakeY[GRID_WIDTH * GRID_HEIGHT];
int appleX = 0;
int appleY = 0;
int snakeDirection = 0; // 0: right, 1: down, 2: left, 3: up
bool snakeGameOver = false;

void playSnake() {
    // Initialize the screen
    display.clearDisplay();
    display.setCursor(0, 0);
    display.display();

    clearScreen();
    snakeScore = 0;
    snakeLength = 1;
    snakeX[0] = GRID_WIDTH / 2;
    snakeY[0] = GRID_HEIGHT / 2;
    snakeDirection = 0;
    snakeGameOver = false;

    // Generate initial position of the apple
    appleX = random(0, GRID_WIDTH);
    appleY = random(0, GRID_HEIGHT);

    // Select player name
    playerName[0] = ' ';
    playerName[1] = ' ';
    playerName[2] = ' ';
    playerName[3] = '\0';
    getPlayerName();

    while (!snakeGameOver) {
        clearScreen();
        // Draw apple as filled square
        int applePx = appleX * CELL_SIZE + 1;
        int applePy = appleY * CELL_SIZE + 1;
        display.fillRect(applePx, applePy, CELL_SIZE - 2, CELL_SIZE - 2, SSD1306_WHITE);
        // Draw snake head as outlined square, body as filled squares
        for (int i = 0; i < snakeLength; i++) {
            int sx = snakeX[i] * CELL_SIZE;
            int sy = snakeY[i] * CELL_SIZE;
            if (i == 0) {
                display.drawRect(sx, sy, CELL_SIZE, CELL_SIZE, SSD1306_WHITE);
            } else {
                display.fillRect(sx + 1, sy + 1, CELL_SIZE - 2, CELL_SIZE - 2, SSD1306_WHITE);
            }
        }
        display.display();

        // Read user input
        if (digitalRead(UP) == LOW && snakeDirection != 1) {
            snakeDirection = 3;
        } else if (digitalRead(DOWN) == LOW && snakeDirection != 3) {
            snakeDirection = 1;
        } else if (digitalRead(LEFT) == LOW && snakeDirection != 0) {
            snakeDirection = 2;
        } else if (digitalRead(RIGHT) == LOW && snakeDirection != 2) {
            snakeDirection = 0;
        }

        // Move the snake
        for (int i = snakeLength - 1; i > 0; i--) {
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }

        if (snakeDirection == 0) {
            snakeX[0]++;
        } else if (snakeDirection == 1) {
            snakeY[0]++;
        } else if (snakeDirection == 2) {
            snakeX[0]--;
        } else if (snakeDirection == 3) {
            snakeY[0]--;
        }

        // Handle collisions with borders
        if (snakeX[0] < 0) {
            snakeX[0] = GRID_WIDTH - 1;
        } else if (snakeX[0] >= GRID_WIDTH) {
            snakeX[0] = 0;
        }

        if (snakeY[0] < 0) {
            snakeY[0] = GRID_HEIGHT - 1;
        } else if (snakeY[0] >= GRID_HEIGHT) {
            snakeY[0] = 0;
        }

        // Handle collisions with the apple
        if (snakeX[0] == appleX && snakeY[0] == appleY) {
            snakeScore++;
            snakeLength++;
            appleX = random(0, GRID_WIDTH);
            appleY = random(0, GRID_HEIGHT);
        }

        // Handle collisions with the snake body
        for (int i = 1; i < snakeLength; i++) {
            if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
                snakeGameOver = true;
                break;
            }
        }

        delay(200);
    }

    // Show final score
    clearScreen();
    display.println("Game Over!");
    display.println("Score: " + String(snakeScore));
    display.println("\nPress any button to \ncontinue");
    display.display();
    while (digitalRead(UP) == HIGH && digitalRead(DOWN) == HIGH && digitalRead(RIGHT) == HIGH) {
        delay(200);
    }

    // Update leaderboard
    updateSnakeLeaderboard(playerName, snakeScore);
    displaySnakeLeaderboard();
    delay(5000);
}

void updateSnakeLeaderboard(String playerName, int score) {
    updateLeaderboard(playerName, score, "sn");
}

void displaySnakeLeaderboard() {
    displayLeaderboard("sn");
}