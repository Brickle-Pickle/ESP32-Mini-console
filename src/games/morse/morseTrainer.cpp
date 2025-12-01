#include "src/games/morse/morseTrainer.h"
#include "src/led/ledFunctions.h"
#include <FS.h>
#include <SPIFFS.h>

// Morse code table
struct MorseChar {
    char letter;
    const char* code;
};

const MorseChar morseTable[] = {
    {'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."}, {'E', "."},
    {'F', "..-."}, {'G', "--."}, {'H', "...."}, {'I', ".."}, {'J', ".---"},
    {'K', "-.-"}, {'L', ".-.."}, {'M', "--"}, {'N', "-."}, {'O', "---"},
    {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."}, {'S', "..."}, {'T', "-"},
    {'U', "..-"}, {'V', "...-"}, {'W', ".--"}, {'X', "-..-"}, {'Y', "-.--"},
    {'Z', "--.."}
};

const int MORSE_TABLE_SIZE = 26;
const int TOTAL_ROUNDS = 10;

// Get morse code for a letter
const char* getMorseCode(char letter) {
    for (int i = 0; i < MORSE_TABLE_SIZE; i++) {
        if (morseTable[i].letter == letter) {
            return morseTable[i].code;
        }
    }
    return "";
}

// Play morse signal with LED feedback
void playMorseSound(char symbol) {
    int duration = (symbol == '.') ? 200 : 600;
    turnLedOn(ledState, LED);
    delay(duration);
    turnLedOff(ledState, LED);
    delay(200);
}

// Display morse code with sound
void displayMorseWithSound(const char* morse) {
    for (int i = 0; morse[i] != '\0'; i++) {
        playMorseSound(morse[i]);
    }
}

void playMorseTrainer() {
    int score = 0;
    
    // Get player name
    playerName[0] = ' ';
    playerName[1] = ' ';
    playerName[2] = ' ';
    playerName[3] = '\0';
    getPlayerName();

    // Show instructions
    clearScreen();
    display.setCursor(0, 0);
    display.println("Morse Trainer");
    display.println("\nListen & identify");
    display.println("the letter");
    display.println("\nUP/DOWN: navigate");
    display.println("RIGHT: select");
    display.println("\nPress UP to start");
    display.display();
    
    while (digitalRead(UP) == HIGH) {
        delay(20);
    }
    delay(500);

    // Game rounds
    for (int round = 0; round < TOTAL_ROUNDS; round++) {
        // Select random letter
        int letterIndex = random(0, MORSE_TABLE_SIZE);
        char correctLetter = morseTable[letterIndex].letter;
        const char* correctMorse = morseTable[letterIndex].code;

        // Show round info
        clearScreen();
        display.setCursor(0, 0);
        display.println("Round " + String(round + 1) + "/" + String(TOTAL_ROUNDS));
        display.println("\nListen carefully...");
        display.display();
        delay(1500);

        // Play morse code
        clearScreen();
        display.setCursor(0, 0);
        display.println("Pattern:");
        display.println(correctMorse);
        display.display();
        delay(500);
        displayMorseWithSound(correctMorse);
        delay(1000);

        // User selection phase
        int selectedLetter = 0;
        bool answered = false;

        while (!answered) {
            clearScreen();
            display.setCursor(0, 0);
            display.println("Pattern: " + String(correctMorse));
            display.println("\nSelect letter: " + String(morseTable[selectedLetter].letter));
            display.println(" UP/DOWN: change, RIGHT: select");
            display.display();

            if (digitalRead(UP) == LOW) {
                selectedLetter--;
                if (selectedLetter < 0) {
                    selectedLetter = MORSE_TABLE_SIZE - 1;
                }
                delay(200);
            } else if (digitalRead(DOWN) == LOW) {
                selectedLetter++;
                if (selectedLetter >= MORSE_TABLE_SIZE) {
                    selectedLetter = 0;
                }
                delay(200);
            } else if (digitalRead(RIGHT) == LOW) {
                answered = true;
                delay(200);
            } else if (digitalRead(LEFT) == LOW) {
                // Replay morse code
                displayMorseWithSound(correctMorse);
                delay(500);
            }
        }

        // Check answer
        bool correct = (morseTable[selectedLetter].letter == correctLetter);
        if (correct) {
            score++;
        }

        // Show result
        clearScreen();
        display.setCursor(0, 0);
        if (correct) {
            display.println("CORRECT!");
        } else {
            display.println("WRONG!");
            display.println("\nCorrect: " + String(correctLetter));
        }
        display.println("\nScore: " + String(score) + "/" + String(round + 1));
        display.display();
        delay(2000);
    }

    // Final score
    clearScreen();
    display.setCursor(0, 0);
    display.println("Game Over!");
    display.println("\nFinal Score:");
    display.println(String(score) + "/" + String(TOTAL_ROUNDS));
    display.println("\nPress any button");
    display.display();
    
    while (digitalRead(UP) == HIGH && digitalRead(DOWN) == HIGH && digitalRead(RIGHT) == HIGH) {
        delay(200);
    }
    delay(300);

    // Update and show leaderboard
    updateMorseLeaderboard(playerName, score);
    displayMorseLeaderboard();
    delay(5000);
}

void updateMorseLeaderboard(String playerName, int score) {
    updateLeaderboard(playerName, score, "mr");
}

void displayMorseLeaderboard() {
    displayLeaderboard("mr");
}
