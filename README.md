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
The keypad is mapped to the keys:
```
1 2 3 4
Q W E R
A S D F
Z X C V
```
You can use backspace to reset / reload the current ROM

You can use +/- (next to backspace) to adjust execution speed

#### TODO:
- Color themes
- Fix CPU usage

#### Note on roms subfolder
Test roms were sourced [here](https://github.com/Timendus/chip8-test-suite)

Games and demos were sourced [here](https://github.com/kripod/chip8-roms) and [here](https://johnearnest.github.io/chip8Archive/?sort=platform)
