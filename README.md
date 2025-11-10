<div align="center">
    <div>
        <img src="https://img.shields.io/badge/MiniConsole-Arduino%20DIY-1e90ff?style=for-the-badge" />
        &nbsp;&nbsp;&nbsp;
        <img src="https://img.shields.io/badge/license-Non%20Commercial%20v1.0-orange?style=for-the-badge" />
        &nbsp;&nbsp;&nbsp;
        <img src="https://img.shields.io/badge/platform-Arduino-2e8b57?style=for-the-badge" />
        &nbsp;&nbsp;&nbsp;
        <img src="https://img.shields.io/badge/language-C++-00599C?style=for-the-badge" />
        &nbsp;&nbsp;&nbsp;
        <img src="https://img.shields.io/badge/models-3D%20STL-8a2be2?style=for-the-badge" />
        &nbsp;&nbsp;&nbsp;
    </div>
    <br>
    <div>
        <img src="https://img.shields.io/github/stars/Brickle-Pickle/ESP32_Mini-console-project?style=social" />
        &nbsp;&nbsp;&nbsp;
        <img src="https://img.shields.io/github/forks/Brickle-Pickle/ESP32_Mini-console-project?style=social" />
    </div>
    <br>
</div>

# MiniConsole
A DIY Arduino-based mini game console that includes simple games, a trivia mode, LED feedback, and lightweight score tracking. The project is modular to make it easy to add new games, LED effects, and hardware tweaks. Models are provided to 3D print the enclosure.

> **TIP:** Use this repository to learn Arduino, extend with your own games, and tweak LED effects.

---

## Features
- Multiple games orchestration and shared helpers in `gamesFunctions.*`
- Trivia mode with questions loaded from `data/questions.tr`
- LED control and animations in `ledFunctions.*`
- Snake game with scoring and leaderboard in `snake.*`
- Centralized configuration in `config.*`
- Common utilities in `utils.*`
- Score files in `data/` and 3D printable models in `models/`

---

## Project Structure
| File/Folder              | Description                                                               |
|--------------------------|---------------------------------------------------------------------------|
| `MiniConsole.ino`        | Arduino firmware entry point                                              |
| `config.cpp`, `config.h` | System configuration and parameters                                       |
| `ledFunctions.*`         | LED control and effects                                                   |
| `gamesFunctions.*`       | Game orchestration and shared logic                                       |
| `trivial.*`              | Trivia game logic                                                         |
| `snake.*`                | Snake game logic and leaderboard handling                                 |
| `reactionTime.*`         | Reaction time game logic and leaderboard handling                         |
| `utils.*`                | Shared utility functions                                                  |
| `data/`                  | Contains trivia questions and score files                                 |
| `models/`                | 3D printable models for the MiniConsole enclosure                         |

---

## Setup
1. Open `MiniConsole.ino` in the Arduino IDE.
2. Select the appropriate board and port.
3. Build and upload the firmware to the board.

---

## Usage
- On boot, the console initializes configuration and LEDs.
- Use the physical controls to select a game or the trivia mode.
- Select Snake in the game menu to play; eat apples to score, avoid the body, edges wrap, press any button after game over to continue.
- Scores are updated and stored in the corresponding `data/` files.

---

## Configuration
- Adjust pins, timing, brightness, and game options in `config.h` and `config.cpp`.
- Add or edit questions in `data/questions.tr` according to the format used by `trivial.*`.
- Reset or edit scores in `data/leaderboard.tr` and `data/leaderboard.sn` as needed.

---

## Data Formats
- Questions and scores are kept in plain text files under `data/`.
- `data/leaderboard.sn` stores Snake scores using `playerName,score`, only the top 3 entries are kept.
- Inspect `trivial.*` files and game code to define exact formats, then update this section.

---

## Models
| Model File               | Description                                                                 |
|--------------------------|-----------------------------------------------------------------------------|
| `MiniConsole.stl`        | Standard 3D model for the MiniConsole enclosure                            |
| `Flat MiniConsole.stl`   | Alternative flat design for the MiniConsole enclosure                      |

---

## Hardware Notes
- Arduino compatible board such as UNO or similar
- LEDs and basic input components as required by the chosen build
- Pinout and any LED library details should be documented in config files

### Necessary Hardware
| Component                 | Quantity | Notes                                                           |
|---------------------------|----------|-----------------------------------------------------------------|
| Arduino compatible board  | 1        | e.g., UNO or Raspberry Pi Pico for additional features         |
| LEDs                      | 1        | e.g., WS2812B                                                  |
| Buttons                   | 4        | Basic input components                                          |
| Joystick                  | 1        | For game controls                                               |
| Resistors                 | 4        | For prototyping                                                 |
| Capacitor                 | 1        | For prototyping                                                 |
| OLED display              | 1        | e.g., SSD1306                                                   |
| Power supply              | 1        | e.g., 5V USB power bank                                         |

---

## Contributing
- Open issues for bugs or improvements.
- Submit PRs that preserve the current style and modular design.
- Include examples or tests when adding new games or LED effects.

---

## License
> **IMPORTANT:** Non-commercial only. Commercial use requires a commercial license. Contact info: info@bricklepickle.com.  
> Models in `models/` are free for any use. Data files in `data/` can be modified but their use and redistribution are non-commercial only.  
> Attribution must include Brickle Pickle and the original repository link: [GitHub Repository](https://github.com/Brickle-Pickle/ESP32_Mini-console-project).  
> Code is available under the MiniConsole Non-Commercial License v1.0. See `LICENSE.md` for the full license text.

## Snake Game
- Source: `MiniConsole\src\games\snake\snake.cpp`, `MiniConsole\src\games\snake\snake.h`.
- Controls: UP, DOWN, LEFT, RIGHT move the snake; movement cannot reverse into itself directly.
- Gameplay: Eat apples to increase score and length; screen edges wrap; collision with the body ends the game.
- Leaderboard: After game over, scores update `data/leaderboard.sn` and display top entries.

## Reaction Time Game
- Source: `MiniConsole\src\games\reaction_Time\reactionTime.cpp`, `MiniConsole\src\games\reaction_Time\reactionTime.h`.
- Controls: Any button to start the game; press the button as quickly as possible.
- Gameplay: The game displays a random sequence of LEDs; the player must press the button as soon as the last LED lights up.
- PvP: Two players may play against each other; the first to press the button wins.

## Trivial Game
- Source: `MiniConsole\src\games\trivial\trivial.cpp`, `MiniConsole\src\games\trivial\trivial.h`.
- Controls: UP, DOWN to navigate questions; RIGHT button to select an answer.
- Gameplay: Players answer trivial questions; correct answers score points.
- Leaderboard: After game over, scores update `data/leaderboard.tr` and display top entries.

## Aditional Documentation
- [How to upload a game](GAMES_UPLOAD.md)