#include "gamesFunctions.h"
#include "config.h"
#include "trivial.h"

void gameMenu() {
    bool exit = false;
    int gameSelectedOption = 0;

    while (!exit) {
        clearScreen();
        printMenu(gameMenuOptions, gameMenuOptionsSize, "GAME MENU:", gameSelectedOption);

        if (digitalRead(UP) == LOW) {
            gameSelectedOption--;
            if (gameSelectedOption < 0)  gameSelectedOption = 0; 
            delay(200);
        } else if (digitalRead(DOWN) == LOW) {
            gameSelectedOption++;
            if (gameSelectedOption >= gameMenuOptionsSize) { 
                gameSelectedOption = gameMenuOptionsSize - 1;
            }
            delay(200);
        } else if (digitalRead(LEFT) == LOW) {
            exit = true;
            delay(200);
        } else if (digitalRead(RIGHT) == LOW) {
            gameMenuController(gameSelectedOption);
            delay(200);
        }
    }
}

void gameMenuController(int selectedOption) {
    clearScreen();
    loadScene(gameMenuOptions[selectedOption].text);
    
    switch (selectedOption) {
        case 0: playTrivial(); break;
        // case 1: playSnake(); break;
        // case 2: playPong(); break;
    }
}