# How to Upload Your Game to MiniConsole

> Follow these steps to make your game stand out and playable on the MiniConsole!

---

## 1. Create Your Game Folder
Create a folder in `MiniConsole/src/games/` and name it after your game.

## 2. Add Source Files
Include at least:
- `yourGameName.cpp` (main implementation)
- `yourGameName.h` (function and variable declarations)

## 3. Data Files (Optional)
If your game needs data (questions, scores, etc.), create a file named `purpose.xx` in your game folder and describe its purpose.
Replace `xx` with the correct extension (e.g., `sn` for Snake, `rt` for Reaction Time).

## 4. Add Your Game to config.cpp
Edit `MiniConsole/src/os/config.cpp` and add your game to `gameMenuOptions`.

## 5. Update gamesFunctions.cpp
In `MiniConsole/src/games/gamesFunctions.cpp`:
- In the `gameMenuController` function, add a `case` for your game in the same order as in `config.cpp`.
- Add the include for your header:
```cpp
#include "src/games/yourGameName/yourGameName.h"
```

## 6. Declare the Main Function in Your Header
In `yourGameName.h`:
- Add at least:
  - `void playYourGameName();`
- Declare any other functions you use.

## 7. Implement the Logic in Your .cpp
In `yourGameName.cpp`, implement the `playYourGameName` function with your game logic.

## 8. Upload the Firmware
Open `MiniConsole.ino` in Arduino IDE, select the correct board and port, build and upload.

## 9. Test Your Game
Play on the MiniConsole and make sure everything works as expected.

## 10. Update Documentation (Optional)
Add details about your game in `README.md`: controls, gameplay, special features, etc.

---

Done! That is how you add and share your game on the MiniConsole.