#ifndef LED_FUNCTIONS_H
#define LED_FUNCTIONS_H

#include "src/os/utils.h"

// LED Menu Functions
void turnLedOn(bool &ledState, int ledPin);
void turnLedOff(bool &ledState, int ledPin);
void blinkLed(bool &ledState, int ledPin);
void ledMenuController(int selectedOption, bool &ledState, int ledPin);
void ledMenu(bool &ledState, int ledPin, MenuOption ledMenuOptions[]);

#endif