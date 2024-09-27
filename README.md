# chip8 interpreter
CHIP-8 Emulator written in C++ and SFML

To build:
``` 
g++ main.cpp chip8.cpp -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -o chip8
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

#### The current state of things:
- Does not have audio
- The random number generation is broken
- It *does* run a handful of games okay, many are hopelessly broken

#### Note on subfolders
The *games* folder was sourced [here](https://www.zophar.net/pdroms/chip8/chip-8-games-pack.html)
The *tests* folder was sourced [here](https://github.com/Timendus/chip8-test-suite)