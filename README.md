# chip8 interpreter
CHIP-8 Emulator written in C++ and SFML

To build:
``` 
gcc main.cpp chip8.cpp -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -o chip8
```

To use:
```
./chip8 ROM_FILE
```

#### The current state of things:
- Does not have audio
- The random number generation is broken
- It *does* run a handful of games okay, many are hopelessly broken