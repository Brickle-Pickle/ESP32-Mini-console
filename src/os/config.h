#ifndef CONFIG_H
#define CONFIG_H

// Imports
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Pins
#define LDR 2
#define LED 21
#define SDA 3
#define SCL 4

#define UP 8
#define DOWN 6
#define RIGHT 7
#define LEFT 5

// Oled Config
#define HEIGHT 32
#define WIDTH 128
extern Adafruit_SSD1306 display;

// Data Structures
struct MenuOption {
    String text;
    int optionValue;
};

struct TrivialQuestion {
    String question;
    String options[3];
    int correctAnswer;
    int _id;
};

// Global Variables
#define TRIVIAL_MAX_QUESTIONS 10
extern int ldrValue;
extern bool ledState;
extern int threshold;
extern int selectedOption;
extern char playerName[3];
extern char lettersAvailable[27];

extern const int TRIVIAL_QUESTIONS_PER_GAME;

// Menu Options
extern MenuOption mainMenuOptions[];
extern MenuOption ledMenuOptions[];
extern MenuOption lightMenuOptions[];
extern MenuOption gameMenuOptions[];

// Menu Options Sizes
extern const int mainMenuOptionsSize;
extern const int ledMenuOptionsSize;
extern const int lightMenuOptionsSize;
extern const int gameMenuOptionsSize;

#endif