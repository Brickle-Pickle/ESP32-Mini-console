#ifndef UTILS_H
#define UTILS_H

#include "src/os/config.h"

void clearScreen();
void loadScene(String scene);
void printMenu(MenuOption menuOptions[], int numOptions, String title, int menuSelectedOption);
void getPlayerName();
void updateLeaderboard(String playerName, int score, char game[2]);
void displayLeaderboard(char game[2]);

#endif