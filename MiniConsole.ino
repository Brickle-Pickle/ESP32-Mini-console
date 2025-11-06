// Imports
#include "config.h"
#include "utils.h"
#include "ledFunctions.h"
#include "esp_sleep.h"
#include "gamesFunctions.h"

// Setup
void setup() {
    // Serial
    Serial.begin(115200);
    delay(4000);
    Serial.println("Started mini console");

    // Pin init
    pinMode(LDR, INPUT);
    pinMode(LED, OUTPUT);
    pinMode(UP, INPUT_PULLUP);
    pinMode(DOWN, INPUT_PULLUP);
    pinMode(RIGHT, INPUT_PULLUP);
    pinMode(LEFT, INPUT_PULLUP);

    // Oled init
    Wire.begin(SDA, SCL);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("Screen init failed"));
        while (1);
    }

    // Display Init
    initDisplay();
}

void loop() {
    clearScreen();
    printMenu(mainMenuOptions, mainMenuOptionsSize, "MAIN MENU:", selectedOption);

    // Handle Main Menu Navigation
    if (digitalRead(UP) == LOW) {
        selectedOption--;
        if (selectedOption < 0) {
            selectedOption = 0;
        }
        delay(200);
    } else if (digitalRead(DOWN) == LOW) {
        selectedOption++;
        if (selectedOption >= mainMenuOptionsSize) {
            selectedOption = mainMenuOptionsSize - 1;
        }
        delay(200);
    } else if (digitalRead(RIGHT) == LOW) {
        delay(200);
        switch (selectedOption) {
            case 0: ledMenu(ledState, LED, ledMenuOptions); break;
            // case 1: lightMenu(); break;
            case 2: gameMenu(); break;
            case 3: exitProgram(); break;
        }
    }
}

void initDisplay() {
    clearScreen();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    loadScene("Starting mini console");
}

void exitProgram() {
    clearScreen();
    loadScene("Exiting mini console");
    delay(2000);
    turnLedOff(ledState, LED);
    esp_sleep_enable_timer_wakeup(0);
    esp_deep_sleep_start();
}