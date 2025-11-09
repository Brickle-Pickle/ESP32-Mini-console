# MiniConsole

A DIY Arduino/ESP32 based mini game console that includes simple games, a trivia mode, LED feedback, and lightweight score tracking. The project is modular to make it easy to add new games, LED effects, and hardware tweaks. Models are provided to 3D print the enclosure.

## Features
- Multiple games orchestration and shared helpers in `gamesFunctions.*`
- Trivia mode with questions loaded from `data/questions.tr`
- LED control and animations in `ledFunctions.*`
- Centralized configuration in `config.*`
- Common utilities in `utils.*`
- Score files in `data/` and 3D printable models in `models/`

## Project Structure
- `MiniConsole.ino`: Arduino firmware entry point
- `config.cpp`, `config.h`: system configuration and parameters
- `ledFunctions.cpp`, `ledFunctions.h`: LED control and effects
- `gamesFunctions.cpp`, `gamesFunctions.h`: game orchestration and shared logic
- `trivial.cpp`, `trivial.h`: trivia game logic
- `utils.cpp`, `utils.h`: shared utility functions
- `data/`:
  - `questions.tr`: trivia questions
  - `leaderboard.tr`: trivia scores
  - `leaderboard.sn`: scores for another game
- `models/`:
  - `MiniConsole.stl`
  - `Flat MiniConsole.stl`

## Hardware Notes
- Arduino compatible board such as UNO or similar
- Raspberry Pi Pico or similar for additional features
- LEDs and basic input components as required by the chosen build
- Pinout and any LED library details should be documented in `config.*` and comments
- Necessary hardware:
  - Arduino compatible board (e.g., UNO) / Raspberry Pi Pico or similar for additional features [x1]
  - LEDs (e.g., WS2812B) [x1]
  - Basic input components (e.g., buttons, joystick) [x4 buttons, x1 joystick]
  - Resistors, capacitors, and breadboard for prototyping [x4 resistors, x1 capacitor]
  - OLED display (e.g., SSD1306) [x1]
  - Power supply (e.g., 5V USB power bank) [x1]

## Setup
1. Open `MiniConsole.ino` in the Arduino IDE
2. Select board and port
3. Build and upload the firmware to the board

## Usage
- On boot, the console initializes configuration and LEDs
- Use the physical controls to select a game or the trivia mode
- Scores are updated and stored in the corresponding `data/` files during development

## Configuration
- Adjust pins, timing, brightness, and game options in `config.h` and `config.cpp`
- Add or edit questions in `data/questions.tr` according to the format used by `trivial.*`
- Reset or edit scores in `data/leaderboard.tr` and `data/leaderboard.sn` as needed

## Data Formats
- Questions and scores are kept in plain text files under `data/`
- Exact formats should be documented by inspecting `trivial.*` and game code, then updated here

## Models
- Use `models/MiniConsole.stl` or `models/Flat MiniConsole.stl` depending on your build
- Adjust print settings to match your printer and material

## Contributing
- Open issues for bugs or improvements
- Submit PRs that preserve the current style and modular design
- Include examples or tests when adding new games or LED effects
