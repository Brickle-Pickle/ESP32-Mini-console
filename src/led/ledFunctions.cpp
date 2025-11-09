#include "src/led/ledFunctions.h"

// Internal Variable
TaskHandle_t blinkLedTaskHandle = NULL;

// Task Declaration
void TaskBlinkLed(void *pvParameters);

void ledMenu(bool &ledState, int ledPin, MenuOption ledMenuOptions[]) {
    bool exit = false;
    int ledSelectedOption = 0;

    while (!exit) {
        clearScreen();
        printMenu(ledMenuOptions, ledMenuOptionsSize, "LED MENU:", ledSelectedOption);

        if (digitalRead(UP) == LOW) {
            ledSelectedOption--;
            if (ledSelectedOption < 0)  ledSelectedOption = 0; 
            delay(200);
        } else if (digitalRead(DOWN) == LOW) {
            ledSelectedOption++;
            if (ledSelectedOption >= ledMenuOptionsSize) {
                ledSelectedOption = ledMenuOptionsSize - 1;
            }
            delay(200);
        } else if (digitalRead(LEFT) == LOW) {
            exit = true;
            delay(200);
        } else if (digitalRead(RIGHT) == LOW) {
            ledMenuController(ledSelectedOption, ledState, ledPin);
            delay(200);
        }
    }
}

void ledMenuController(int selectedOption, bool &ledState, int ledPin) {
    switch (selectedOption) {
        case 0: turnLedOn(ledState, ledPin); break;
        case 1: turnLedOff(ledState, ledPin); break;
        case 2: blinkLed(ledState, ledPin); break;
    }
}

void turnLedOn(bool &ledState, int ledPin) {
    if (blinkLedTaskHandle != NULL) {
        vTaskDelete(blinkLedTaskHandle);
        blinkLedTaskHandle = NULL;
    }
    digitalWrite(ledPin, HIGH);
    ledState = true;
}

void turnLedOff(bool &ledState, int ledPin) {
    if (blinkLedTaskHandle != NULL) {
        vTaskDelete(blinkLedTaskHandle);
        blinkLedTaskHandle = NULL;
    }
    digitalWrite(ledPin, LOW);
    ledState = false;
}

void blinkLed(bool &ledState, int ledPin) {
    if (blinkLedTaskHandle == NULL) {
       xTaskCreate(
            TaskBlinkLed,
            "BlinkLedTask",
            1000,
            (void*)&ledPin,
            1,
            &blinkLedTaskHandle
        );
    }
}

void TaskBlinkLed(void *pvParameters) {
    int ledPin = *((int*)pvParameters);
    while (true) {
        digitalWrite(ledPin, HIGH);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        digitalWrite(ledPin, LOW);
        vTaskDelay(1500 / portTICK_PERIOD_MS);
    }
}