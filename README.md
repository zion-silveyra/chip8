# chip8 interpreter
CHIP-8 Emulator written in C++ and SFML

#### Instructions
To build:
``` 
make chip8
```

To test:
```
make all
```

To use:
```
./chip8 ROM_FILE
```
#### Controls
The keyboard is mapped to the keypad as such:
```
1 2 3 4     1 2 3 C
Q W E R  => 4 5 6 D
A S D F  => 7 8 9 E
Z X C V     A 0 B F
```
<kbd>Bksp</kbd> resets the interpreter, and reloads the current ROM

<kbd>+</kbd>/<kbd>-</kbd> keys adjust the execution speed

<kbd>\\</kbd> cycles through five color themes

<kbd>Enter</kbd> inverts the sprite and background colors

#### TODO:
- Color themes
- Fix CPU usage

#### Note on roms subfolder
Test roms were sourced [here](https://github.com/Timendus/chip8-test-suite)

Games and demos were sourced [here](https://github.com/kripod/chip8-roms) and [here](https://johnearnest.github.io/chip8Archive/?sort=platform)
