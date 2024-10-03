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

#### TODO:
- Configurable display size
- ROM compatibility is like, 20%, that isn't great
- Fix CPU usage

#### Note on subfolders
The *games* folder was sourced [here](https://www.zophar.net/pdroms/chip8/chip-8-games-pack.html)

The *tests* folder was sourced [here](https://github.com/Timendus/chip8-test-suite)