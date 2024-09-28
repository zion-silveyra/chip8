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

#### TODO:
- Separate clock and delay/sound timers
- Configurable execution speed
- Configurable display size
- ROM compatibility is like, 20%, that isn't great

#### Note on subfolders
The *games* folder was sourced [here](https://www.zophar.net/pdroms/chip8/chip-8-games-pack.html)

The *tests* folder was sourced [here](https://github.com/Timendus/chip8-test-suite)