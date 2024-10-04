all: chip8 test-display test-cpu test-quirks test-io

chip8: main.cpp chip8.cpp
	g++ main.cpp chip8.cpp -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -o chip8

test-display: chip8 roms/tests/1-chip8-logo.ch8 roms/tests/2-ibm-logo.ch8
	./chip8 roms/tests/1-chip8-logo.ch8
	./chip8 roms/tests/2-ibm-logo.ch8

test-cpu: chip8 roms/tests/3-corax+.ch8 roms/tests/4-flags.ch8
	./chip8 roms/tests/3-corax+.ch8
	./chip8 roms/tests/4-flags.ch8

test-quirks: chip8 roms/tests/5-quirks.ch8
	./chip8 roms/tests/5-quirks.ch8

test-io: chip8 roms/tests/6-keypad.ch8 roms/tests/7-beep.ch8
	./chip8 roms/tests/6-keypad.ch8
	./chip8 roms/tests/7-beep.ch8

clean:
	rm chip8
