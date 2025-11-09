#include "utils.h"
#include <FS.h>
#include <SPIFFS.h>

void clearScreen() {
    display.clearDisplay();
    display.setCursor(0, 0);
}

void loadScene(String text){
    int percent = 0;

    // Load Scene Text
    while (percent < 128) {
        display.println(text);
        display.fillRect(0, 22, percent, 8, SSD1306_WHITE);
        display.drawRect(0, 22, 128, 8, SSD1306_WHITE);
        display.display();
        delay(20);
        clearScreen();
        percent++;
    }
}

void printMenu(MenuOption menuOptions[], int size, String title, int menuSelectedOption) {
    // Get Final Option Index
    int finalOp;
    finalOp = (menuSelectedOption == 0) ? 1 : (menuSelectedOption == size - 1) ? size - 2 : menuSelectedOption;
    int pointerIndex = (menuSelectedOption == 0) ? -1 : (menuSelectedOption == size - 1) ? 1 : 0;

    // Print Menu Options
    display.println(title);
    
    for (int i = -1; i <= 1; i++) {
        int idx = finalOp + i;
        if (idx < 0 || idx >= size) continue;
        String prefix = (i == pointerIndex) ? "   >  " : "      ";
        display.println(prefix + menuOptions[idx].text);
    }

    display.display();
}

void printMenu(char menuOptions[], int size, String title, char currentInput[], int menuSelectedOption) {
    // Get Final Option Index
    int finalOp;
    finalOp = (menuSelectedOption == 0) ? 1 : (menuSelectedOption == size - 1) ? size - 2 : menuSelectedOption;
    int pointerIndex = (menuSelectedOption == 0) ? -1 : (menuSelectedOption == size - 1) ? 1 : 0;

    // Print Menu Options
    display.println(title + String(currentInput));
    
    for (int i = -1; i <= 1; i++) {
        int idx = finalOp + i;
        if (idx < 0 || idx >= size) continue;
        String prefix = (i == pointerIndex) ? "   >  " : "      ";
        display.println(prefix + menuOptions[idx]);
    }

    display.display();
}

void getPlayerName() {
    bool nameCompleted = false;
    while (!nameCompleted) {
        int selectedLetter = 0;
        int nameLength = 0;
        
        while (nameLength < 3) {
            clearScreen();
            printMenu(lettersAvailable, 27, "Enter your name: ", playerName, selectedLetter);
            
            if (digitalRead(UP) == LOW) {
                selectedLetter--;
                if (selectedLetter < 0) {
                    selectedLetter = 0;
                }
                delay(200);
            } else if (digitalRead(DOWN) == LOW) {
                selectedLetter++;
                if (selectedLetter >= 27) {
                    selectedLetter = 27 - 1;
                }
                delay(200);
            } else if (digitalRead(RIGHT) == LOW) {
                playerName[nameLength] = lettersAvailable[selectedLetter];
                nameLength++;
                delay(200);
            }
        }

        clearScreen();
        nameCompleted = true;
        display.println("Name: " + String(playerName));
        display.println("\nPress any button to \ncontinue");
        display.display();
        while (digitalRead(UP) == HIGH && digitalRead(DOWN) == HIGH && digitalRead(RIGHT) == HIGH) {
            delay(200);
        }
        delay(200);
    }
}

/* 
    Update the leaderboard game file. FILE: leaderboard.(trivial = .tr, snake = .sn, pong = .po)
    Format: playerName,score
    Only the 3 best scores are saved.
*/
void updateLeaderboard(String playerName, int score, char game[2]) {
    String fileName = "/leaderboard." + String(game);
    Serial.println(String("DEBUG updateLeaderboard file ") + fileName + String(" newEntry ") + playerName + String(",") + String(score));
    String bestPlayers[3] = {"", "", ""};
    int bestScores[3] = {0, 0, 0};
    File leaderboardFile = SPIFFS.open(fileName, "r");
    if (leaderboardFile) {
        String line;
        while (leaderboardFile.available()) {
            line = leaderboardFile.readStringUntil('\n');
            Serial.println(String("DEBUG read line ") + line);
            int commaIndex = line.indexOf(',');
            Serial.println(String("DEBUG commaIndex ") + String(commaIndex));
            if (commaIndex < 0) {
                Serial.println("DEBUG bad format no comma");
                continue;
            }
            String player = line.substring(0, commaIndex);
            int playerScore = line.substring(commaIndex + 1).toInt();
            Serial.println(String("DEBUG parsed player ") + player + String(" score ") + String(playerScore));
            if (playerScore > bestScores[0]) {
                bestScores[2] = bestScores[1];
                bestPlayers[2] = bestPlayers[1];
                bestScores[1] = bestScores[0];
                bestPlayers[1] = bestPlayers[0];
                bestScores[0] = playerScore;
                bestPlayers[0] = player;
                Serial.println(String("DEBUG new top1 ") + bestPlayers[0] + String(" ") + String(bestScores[0]));
            } else if (playerScore > bestScores[1]) {
                bestScores[2] = bestScores[1];
                bestPlayers[2] = bestPlayers[1];
                bestScores[1] = playerScore;
                bestPlayers[1] = player;
                Serial.println(String("DEBUG new top2 ") + bestPlayers[1] + String(" ") + String(bestScores[1]));
            } else if (playerScore > bestScores[2]) {
                bestScores[2] = playerScore;
                bestPlayers[2] = player;
                Serial.println(String("DEBUG new top3 ") + bestPlayers[2] + String(" ") + String(bestScores[2]));
            }
        }
        leaderboardFile.close();
    } else {
        Serial.println("DEBUG updateLeaderboard open failed for read");
    }
    if (score > bestScores[0]) {
        bestScores[2] = bestScores[1];
        bestPlayers[2] = bestPlayers[1];
        bestScores[1] = bestScores[0];
        bestPlayers[1] = bestPlayers[0];
        bestScores[0] = score;
        bestPlayers[0] = playerName;
        Serial.println(String("DEBUG new top1 from entry ") + bestPlayers[0] + String(" ") + String(bestScores[0]));
    } else if (score > bestScores[1]) {
        bestScores[2] = bestScores[1];
        bestPlayers[2] = bestPlayers[1];
        bestScores[1] = score;
        bestPlayers[1] = playerName;
        Serial.println(String("DEBUG new top2 from entry ") + bestPlayers[1] + String(" ") + String(bestScores[1]));
    } else if (score > bestScores[2]) {
        bestScores[2] = score;
        bestPlayers[2] = playerName;
        Serial.println(String("DEBUG new top3 from entry ") + bestPlayers[2] + String(" ") + String(bestScores[2]));
    }
    Serial.println(String("DEBUG best players computed ") + bestPlayers[0] + String(" ") + String(bestScores[0]) + String(" | ") + bestPlayers[1] + String(" ") + String(bestScores[1]) + String(" | ") + bestPlayers[2] + String(" ") + String(bestScores[2]));
    File outFile = SPIFFS.open(fileName, "w");
    if (!outFile) {
        Serial.println("DEBUG updateLeaderboard open failed for write");
        display.println("Error opening leaderboard file");
        display.display();
        return;
    }
    Serial.println("DEBUG writing best players");
    outFile.println(bestPlayers[0] + "," + String(bestScores[0]));
    outFile.println(bestPlayers[1] + "," + String(bestScores[1]));
    outFile.println(bestPlayers[2] + "," + String(bestScores[2]));
    outFile.close();
    Serial.println("DEBUG updateLeaderboard done");
}

void displayLeaderboard(char game[2]) {
    clearScreen();
    display.println("Best Players:");
    String fileName = "/leaderboard." + String(game);
    Serial.println(String("DEBUG displayLeaderboard file ") + fileName);
    File leaderboardFile = SPIFFS.open(fileName, "r");
    if (!leaderboardFile) {
        Serial.println("DEBUG displayLeaderboard open failed");
        display.println("Error opening leaderboard file!");
        display.display();
        return;
    }
    Serial.println("DEBUG displayLeaderboard open ok");
    while (leaderboardFile.available()) {
        String line = leaderboardFile.readStringUntil('\n');
        Serial.println(String("DEBUG line raw ") + line);
        int commaIndex = line.indexOf(',');
        Serial.println(String("DEBUG commaIndex ") + String(commaIndex));
        if (commaIndex < 0) {
            Serial.println("DEBUG bad format no comma");
            continue;
        }
        String player = line.substring(0, commaIndex);
        int playerScore = line.substring(commaIndex + 1).toInt();
        Serial.println(String("DEBUG parsed player ") + player + String(" score ") + String(playerScore));
        if (playerScore > 0) {
            display.println("   " + player + " - " + String(playerScore));
        }
    }
    Serial.println("DEBUG displayLeaderboard done");
    leaderboardFile.close();
    display.display();
}