#include "trivial.h"
#include <FS.h>
#include <SPIFFS.h>
#include <esp_system.h>

int trivialScore = 0;
int trivialQuestionCount = 0;
int trivialSelectedQuestion = 0;
int trivialQuestionsSize = 0;
int trivialAlreadyAnsweredQuestions[TRIVIAL_MAX_QUESTIONS] = {0, 0, 0};
bool trivialGameOver = false;

TrivialQuestion trivialQuestions[TRIVIAL_MAX_QUESTIONS];

void playTrivial() {
    setupTrivialData();
    clearScreen();
    int trivialSelectedOption = 0;
    getTrivialQuestions();
    Serial.println("Starting Trivial game...");
    Serial.print("Questions loaded: ");
    Serial.println(trivialQuestionsSize);
    if (trivialQuestionsSize <= 0) {
        display.clearDisplay();
        display.setCursor(0,0);
        display.println("No questions found");
        display.display();
        Serial.println("No questions found in SPIFFS");
        trivialGameOver = true;
        return;
    }
    playerName[0] = ' '; playerName[1] = ' '; playerName[2] = ' '; playerName[3] = '\0';
    getPlayerName();
    
    while (!trivialGameOver) {
        trivialSelectedQuestion = getRandomTrivialQuestion();
        Serial.print("Showing question: ");
        Serial.println(trivialSelectedQuestion);
        display.clearDisplay();
        display.setCursor(0,0);
        displayTrivialQuestion(trivialQuestions[trivialSelectedQuestion], TRIVIAL_MAX_QUESTIONS, trivialSelectedOption);

        int trivialSelectedAnswer = -1;

        while (trivialSelectedAnswer == -1) {
            clearScreen();
            displayTrivialQuestion(trivialQuestions[trivialSelectedQuestion], TRIVIAL_MAX_QUESTIONS, trivialSelectedOption);

            // Handle Trivial Navigation
            if (digitalRead(UP) == LOW) {
                trivialSelectedOption--;
                if (trivialSelectedOption < 0) {
                    trivialSelectedOption = 0;
                }
                delay(200);
            } else if (digitalRead(DOWN) == LOW) {
                trivialSelectedOption++;
                if (trivialSelectedOption >= TRIVIAL_MAX_QUESTIONS) {
                    trivialSelectedOption = TRIVIAL_MAX_QUESTIONS - 1;
                }
                delay(200);
            } else if (digitalRead(RIGHT) == LOW) {
                delay(200);
                trivialSelectedAnswer = trivialSelectedOption;
                if (trivialSelectedAnswer == trivialQuestions[trivialSelectedQuestion].correctAnswer) {
                    trivialScore++;
                    clearScreen();
                    display.println("Correct!");
                    display.display();
                    delay(2000);
                } else {
                    clearScreen();
                    display.println("Incorrect!");
                    display.display();
                    delay(2000);
                }
                clearScreen();
                trivialAlreadyAnsweredQuestions[trivialSelectedQuestion] = 1;
                trivialQuestionCount++;
                
                if (trivialQuestionCount >= TRIVIAL_MAX_QUESTIONS) {
                    trivialGameOver = true;
                }
            }  
        }
    }

    clearScreen();
    display.println("Game Over!");
    display.println("Score: " + String(trivialScore));
    display.println("\nPress any button to \ncontinue");
    display.display();
    while (digitalRead(UP) == HIGH && digitalRead(DOWN) == HIGH && digitalRead(RIGHT) == HIGH) {
        delay(200);
    }

    updateLeaderboard(playerName, trivialScore, "tr");
    displayLeaderboard("tr");
    delay(5000);
}

int getRandomTrivialQuestion() {
    int maxRange = (trivialQuestionsSize > 0) ? trivialQuestionsSize : TRIVIAL_MAX_QUESTIONS;
    int randomQuestion = random(0, maxRange);
    while (trivialAlreadyAnsweredQuestions[randomQuestion] == 1) {
        randomQuestion = random(0, maxRange);
    }
    return randomQuestion;
}

// Load questions from file questions.tr
void getTrivialQuestions() {
    trivialQuestionsSize = 0;
    // Start SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS mount failed");
        return;
    }

    Serial.println("SPIFFS mounted successfully");

    // Open questions file
    File file = SPIFFS.open("/questions.tr", "r");
    if (!file) {
        Serial.println("Failed to open questions file");
        return;
    }
    
    Serial.println("Questions file opened successfully");

    // Read questions from file
    int questionIndex = 0;
    while (file.available() && questionIndex < TRIVIAL_MAX_QUESTIONS) {
        String line = file.readStringUntil('\n');
        if (line.length() > 0) {
            Serial.print("Loading question ");
            Serial.print(questionIndex);
            Serial.print(": ");
            Serial.println(line);
            renderTrivialQuestion(line, questionIndex);
            questionIndex++;
            trivialQuestionsSize++;
        }
    }
    file.close();
    Serial.print("Total questions loaded: ");
    Serial.println(trivialQuestionsSize);
}

void renderTrivialQuestion(String line, int questionIndex) {
    line.trim();
    int questionEndIndex = line.indexOf("_");
    if (questionEndIndex == -1) {
        return;
    }
    trivialQuestions[questionIndex].question = line.substring(0, questionEndIndex);
    line.remove(0, questionEndIndex + 1);
    String tokens[5];
    int tokenIndex = 0;
    while (line.length() > 0 && tokenIndex < 5) {
        int commaPos = line.indexOf(",");
        if (commaPos == -1) {
            tokens[tokenIndex++] = line;
            break;
        } else {
            tokens[tokenIndex++] = line.substring(0, commaPos);
            line.remove(0, commaPos + 1);
        }
    }
    trivialQuestions[questionIndex].options[0] = tokens[0];
    trivialQuestions[questionIndex].options[1] = tokens[1];
    int hyPos = tokens[2].lastIndexOf("-");
    if (hyPos != -1) {
        trivialQuestions[questionIndex].options[2] = tokens[2].substring(0, hyPos);
        trivialQuestions[questionIndex].correctAnswer = tokens[2].substring(hyPos + 1).toInt();
        trivialQuestions[questionIndex]._id = (tokenIndex > 3) ? tokens[3].toInt() : questionIndex;
    } else {
        trivialQuestions[questionIndex].options[2] = tokens[2];
        trivialQuestions[questionIndex].correctAnswer = (tokenIndex > 3) ? tokens[3].toInt() : 0;
        trivialQuestions[questionIndex]._id = (tokenIndex > 4) ? tokens[4].toInt() : questionIndex;
    }
}

void displayTrivialQuestion(TrivialQuestion question, int size, int trivialSelectedQuestion) {
    if (size <= 0 || trivialQuestionsSize <= 0) {
        display.println("Error: No questions loaded");
        display.display();
        Serial.println("Error: No questions available to display");
        return;
    }

    int finalOp;
    finalOp = (trivialSelectedQuestion == 0) ? 1 : (trivialSelectedQuestion == size - 1) ? size - 2 : trivialSelectedQuestion;
    int pointerIndex = (trivialSelectedQuestion == 0) ? -1 : (trivialSelectedQuestion == size - 1) ? 1 : 0;

    // Print Menu Options
    display.println(question.question);
    
    for (int i = -1; i <= 1; i++) {
        int idx = finalOp + i;
        if (idx < 0 || idx >= size) continue;
        String prefix = (i == pointerIndex) ? " >  " : "    ";
        display.println(prefix + question.options[idx]);
    }

    display.display();
}

void setupTrivialData() {
    trivialScore = 0;
    trivialQuestionCount = 0;
    trivialSelectedQuestion = 0;
    trivialQuestionsSize = 0;
    for (int i = 0; i < TRIVIAL_MAX_QUESTIONS; i++) {
        trivialAlreadyAnsweredQuestions[i] = 0;
    }
    trivialGameOver = false;

    // Seed RNG so consecutive game sessions don't produce the same question sequence
    randomSeed(esp_random());
}