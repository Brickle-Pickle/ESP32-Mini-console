#include "src/games/reaction_time/reactionTime.h"
#include "src/led/ledFunctions.h"

int scores[2];
int winner = -1;
int loser = -1;

TaskHandle_t earlyPressTaskHandle = NULL;
void TaskDetectEarlyPress(void *pvParameters);

void playReaction() {
    // Reset scores
    scores[0] = 0;
    scores[1] = 0;
    winner = -1;
    loser = -1;

    turnLedOff(ledState, LED);

    // Initial message
    clearScreen();
    display.setCursor(0,0);
    display.println("Get ready...");
    display.display();
    delay(2000);

    for (int round = 0; round < 5; round++) {
        clearScreen();
        display.println("Round " + String(round + 1));
        display.println("Wait for it...");
        
        // Start early press detection task
        xTaskCreate(
            TaskDetectEarlyPress,
            "EarlyPressTask",
            2048,
            NULL,
            1,
            &earlyPressTaskHandle
        );

        display.display();
        delay(random(3000, 10000));

        clearScreen();
        display.println("Press NOW!");
        turnLedOn(ledState, LED);
        display.display();

        // Wait for button press
        while (digitalRead(RIGHT) == HIGH && digitalRead(LEFT) == HIGH && loser == -1) {
            // Do nothing, just wait till a button is pressed
        }

        turnLedOff(ledState, LED);
        
        if (digitalRead(RIGHT) == LOW) {
            scores[1] ++; // Inverse scoring for right button
            winner = 1;
        } else if (digitalRead(LEFT) == LOW) {
            scores[0] ++; // Inverse scoring for left button
            winner = 0;
        }
        if (loser != -1) { loser = -1; } // Reset loser for next round

        // Eliminate early press task if running
        if (earlyPressTaskHandle != NULL) {
            vTaskDelete(earlyPressTaskHandle);
            earlyPressTaskHandle = NULL;
        }

        // Display round results
        clearScreen();
        winner == 0 ? display.println("Left Player wins this round!") : display.println("Right Player wins this round!");
        winner == 0 ? display.println("Left player score: " + String(scores[0])) : display.println("Right player score: " + String(scores[1]));
        winner = -1;
        display.display();
        delay(2000);
    }

    // Final results
    clearScreen();
    scores[0] > scores[1] ? display.println("Left Player Wins!!") : display.println("Right Player Wins!!");
    display.println("\nPress any button to \ncontinue");
    display.display();
    while (digitalRead(UP) == HIGH && digitalRead(DOWN) == HIGH && digitalRead(RIGHT) == HIGH) {
        delay(20);
    }
}

void TaskDetectEarlyPress(void *pvParameters) {
    // This task will monitor for early button presses
    while (true) {
        if (digitalRead(RIGHT) == LOW || digitalRead(LEFT) == LOW) {
            // Handle early press
            clearScreen();
            display.println("Early Press Detected!");
            loser = (digitalRead(RIGHT) == LOW) ? 1 : 0;
            scores[loser] -= 1; // Penalize loser
            display.println("Loser Score: " + String(scores[loser]));
            display.display();
            delay(2000);
        }
        vTaskDelay(20 / portTICK_PERIOD_MS); // Check every 20ms
    }
}