#include "src/light/light.h"

// Show the light menu until user exits
void lightMenu() {
    bool exit = false;
    while (!exit) {
        clearScreen();
        showLightIntensity();
        display.display();
        // update global ldr value
        ldrValue = analogRead(LDR);
        if (digitalRead(LEFT) == LOW) {
            exit = true;
            delay(200);
        }
        delay(200);
    }
}

// Read LDR on pin and render text plus progress bar
void showLightIntensity() {
    int analogVal = analogRead(LDR);
    if (analogVal < 0) analogVal = 0;
    if (analogVal > 4095) analogVal = 4095;
    int percent = map(analogVal, 0, 4095, 100, 0);
    if (percent < 0) percent = 0;
    if (percent > 100) percent = 100;
    display.println("Luz actual: " + String(percent) + "%");
    int barWidth = map(percent, 0, 100, 0, 128);
    display.fillRect(0, 22, barWidth, 8, SSD1306_WHITE);
    display.drawRect(0, 22, 128, 8, SSD1306_WHITE);
}
