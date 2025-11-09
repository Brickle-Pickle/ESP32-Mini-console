#include "config.h"

// OLED Init
Adafruit_SSD1306 display(WIDTH, HEIGHT, &Wire, -1);

// Global Variables
int ldrValue = 0;
bool ledState = false;
int threshold = 2750;
int selectedOption = 0;
char playerName[3] = "";
char lettersAvailable[27] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', ' '};

const int TRIVIAL_QUESTIONS_PER_GAME = 3;

// Menu Options
MenuOption mainMenuOptions[] = {
    {"LED", 0},
    {"LIGHT", 1},
    {"GAMES", 2},
    {"EXIT", 3}
};
const int mainMenuOptionsSize = sizeof(mainMenuOptions) / sizeof(mainMenuOptions[0]);

MenuOption ledMenuOptions[] = {
    {"LED ON", 1},
    {"LED OFF", 2},
    {"BLINK LED", 3},
};
const int ledMenuOptionsSize = sizeof(ledMenuOptions) / sizeof(ledMenuOptions[0]);

MenuOption lightMenuOptions[] = {
    {"ACTUAL DAY STATE", 1},
    {"LIGHT INTENSITY", 2},
    {"EXIT", 3}
};
const int lightMenuOptionsSize = sizeof(lightMenuOptions) / sizeof(lightMenuOptions[0]);

MenuOption gameMenuOptions[] = {
    {"TRIVIAL", 1},
    {"SNAKE", 2},
    {"PONG", 3}
};
const int gameMenuOptionsSize = sizeof(gameMenuOptions) / sizeof(gameMenuOptions[0]);