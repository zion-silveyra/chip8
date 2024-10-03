#include "chip8.h"

#include <random>
#include <fstream>
#include <array>
#include <cstdint>
#include <iostream>

void chip8::reset()
{
    std::fill(std::begin(display), std::end(display), 0);
    std::fill(std::begin(mem), std::end(mem), 0);
    std::fill(std::begin(v), std::end(v), 0);
    std::fill(std::begin(stack), std::end(stack), 0);
    pc = 0x200;
    sp = 0;
    delay = 0;
    sound = 0;
    index = 0;

    loadFonts();
}

void chip8::loadFonts()
{
    const unsigned int FONTSET_SIZE = 80;

    uint8_t fontset[FONTSET_SIZE] =
    {
	    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	    0x20, 0x60, 0x20, 0x20, 0x70, // 1
	    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    std::copy(std::begin(fontset), std::end(fontset), &mem[0x50]);
}

void chip8::loadProgram(char const* filename)
{
    std::ifstream ifstream;
    ifstream.open(filename, std::ios::binary);

    char buf[0xFFF-0x200];
    std::fill(std::begin(buf), std::end(buf), 0);

    ifstream.read(buf, 0x0FFF-0x200);

    std::copy(std::begin(buf), std::end(buf), &mem[0x200]);

    //std::cout << "\nMemory dump after program load:\n";
    int w = 0;
    for (int i=0;i<4096;++i) {
        //std::cout << std::hex << (uint16_t)mem[i] << " ";
        if (++w == 16) {
            //std::cout << std::endl;
            w=0;
        }
    }

}

void chip8::runTimers()
{
    decrementTimers = true;
}

bool chip8::audioIsPlaying()
{
    return sound != 0;
}

void chip8::runCycle()
{
    // read instruction from pc, pc += 2
    // determine instruction and arguments
    // call function
    // if ext timer-- flag set, do so   

    if (decrementTimers) {
        if (delay > 0)  --delay;
        if (sound > 0)  --sound;
        decrementTimers = false;
    }
    
    //std::cout << "fetch from pc : " << pc;

    uint16_t instr = (mem[pc] << 8) | mem[pc+1];
    pc += 2;

    //std::cout << " ... exec instr : ";
    //std::cout << std::hex << instr << std::endl;
    executeInstruction(instr);

}

void chip8::executeInstruction(uint16_t instr)
{
    // populate most common arguments, but not all are used

    uint8_t regX, regY;
    uint16_t addr;
    uint8_t imm;

    regX = (instr & 0x0F00) >> 8;
    regY = (instr & 0x00F0) >> 4;
    addr = instr & 0x0FFF;
    imm  = instr & 0x00FF;

    // narrow down opcodes by top nibble

    switch ((instr & 0xF000) >> 12) {
    case 0x0000:
        if      (instr == 0x00e0) op_00e0();
        else if (instr == 0x00ee) op_00ee();
        break;
    case 0x1:
        op_1nnn(addr);
        break;
    case 0x2:
        op_2nnn(addr);
        break;
    case 0x3:
        op_3xkk(regX, imm);
        break;
    case 0x4:
        op_4xkk(regX, imm);
        break;
    case 0x5:
        // maybe check for end in zero
        op_5xy0(regX, regY);
        break;
    case 0x6:
        op_6xkk(regX, imm);
        break;
    case 0x7:
        op_7xkk(regX, imm);
        break;
    case 0x8:
        switch (instr & 0x000F) {
        case 0x0:
            op_8xy0(regX, regY);
            break;
        case 0x1:
            op_8xy1(regX, regY);
            break;
        case 0x2:
            op_8xy2(regX, regY);
            break;
        case 0x3:
            op_8xy3(regX, regY);
            break;
        case 0x4:
            op_8xy4(regX, regY);
            break;
        case 0x5:
            op_8xy5(regX, regY);
            break;
        case 0x6:
            op_8xy6(regX, regY);
            break;
        case 0x7:
            op_8xy7(regX, regY);
            break;
        case 0xe:
            op_8xye(regX, regY);
            break;    
        }
        break;
    case 0x9:
        op_9xy0(regX, regY);
        break;
    case 0xa:
        op_annn(addr);
        break;
    case 0xb:
        op_bnnn(addr);
        break;
    case 0xc:
        op_cxkk(regX, imm);
        break;
    case 0xd:
        imm &= 0x0F;
        op_dxyn(regX, regY, imm);
        break;
    case 0xe:
        if ((instr & 0x00FF) == 0x9e) op_ex9e(regX);
        if ((instr & 0x00FF) == 0xa1) op_exa1(regX);
        break;
    case 0xf:
        switch (instr & 0x00FF) {
        case 0x07:
            op_fx07(regX);
            break;
        case 0x0a:
            op_fx0a(regX);
            break;
        case 0x15:
            op_fx15(regX);
            break;
        case 0x18:
            op_fx18(regX);
            break;
        case 0x1e:
            op_fx1e(regX);
            break;
        case 0x29:
            op_fx29(regX);
            break;
        case 0x33:
            op_fx33(regX);
            break;
        case 0x55:
            op_fx55(regX);
            break;
        case 0x65:
            op_fx65(regX);
            break;
        }
        break;
    }
}

// Clear display
void chip8::op_00e0()
{
    std::fill(std::begin(display), std::end(display), 0);
}

// Return from subroutine
void chip8::op_00ee()
{
    --sp;
    pc = stack[sp];
}

// jump
void chip8::op_1nnn(uint16_t addr)
{
    pc = addr;
}

// call subroutine 
void chip8::op_2nnn(uint16_t addr)
{
    stack[sp] = pc;
    ++sp;
    pc = addr;
}

// skip if equal
void chip8::op_3xkk(uint8_t reg, uint8_t imm)
{
    if (v[reg] == imm) pc += 2;
}

// skip if not equal
void chip8::op_4xkk(uint8_t reg, uint8_t imm)
{
    if (v[reg] != imm) pc += 2;
}

// skip if equal reg, reg
void chip8::op_5xy0(uint8_t regA, uint8_t regB)
{
    if (v[regA] == v[regB]) pc += 2;
}

// load byte
void chip8::op_6xkk(uint8_t reg, uint8_t imm)
{
    v[reg] = imm;
}

// add immediate
void chip8::op_7xkk(uint8_t reg, uint8_t imm)
{
    v[reg] += imm;
}

// copy register
void chip8::op_8xy0(uint8_t regA, uint8_t regB)
{
    v[regA] = v[regB];
    v[0xf] = 0;
}

// vx = vx | vy
void chip8::op_8xy1(uint8_t regA, uint8_t regB)
{
    v[regA] |= v[regB];
    v[0xf] = 0;
}

// vx = vx & vy
void chip8::op_8xy2(uint8_t regA, uint8_t regB)
{
    v[regA] &= v[regB];
    v[0xf] = 0;
}

// xor
void chip8::op_8xy3(uint8_t regA, uint8_t regB)
{
    v[regA] ^= v[regB];
    v[0xf] = 0;
}

// add
void chip8::op_8xy4(uint8_t regA, uint8_t regB)
{
    uint16_t wide_result{ v[regA] + v[regB]};
    uint8_t result{ v[regA] + v[regB]};

    v[regA] = result;

    if (wide_result != (uint16_t)result) v[0xf] = 1;
    else v[0xf] = 0;
    
    
}

// sub
void chip8::op_8xy5(uint8_t regA, uint8_t regB)
{
    uint8_t setFlag{0x01};

    if (v[regA] < v[regB]) {
        setFlag &= 0x00;
    }

    v[regA] = v[regA] - v[regB];

    v[0xf] = setFlag;
}

// shift right
void chip8::op_8xy6(uint8_t regA, uint8_t regB)
{
    uint8_t setFlag{};

    if (v[regB] & 0x01)
        setFlag |= 0x01;

    v[regA] = v[regB] >> 1;
    
    v[0xf] = setFlag;
}

// subn

void chip8::op_8xy7(uint8_t regA, uint8_t regB)
{
    uint8_t setFlag{0x01};

    if (v[regA] > v[regB]) {
        setFlag &= 0x00;
    }

    v[regA] = v[regB] - v[regA];

    v[0xf] = setFlag;
}

// shift left
void chip8::op_8xye(uint8_t regA, uint8_t regB)
{
    uint8_t setFlag{};

    if (v[regB] & 0x80)
        setFlag |= 0x01;

    v[regA] = v[regB] << 1;

    v[0xf] = setFlag;
}

// skip if not equal (reg, reg)
void chip8::op_9xy0(uint8_t regA, uint8_t regB)
{
    if (v[regA] != v[regB])
        pc+=2;
}

// load -> index
void chip8::op_annn(uint16_t addr)
{
    index = addr;
}

// jump offset (v0 + addr)
void chip8::op_bnnn(uint16_t addr)
{
    pc = v[0] + addr;
}

// random number, then AND 
void chip8::op_cxkk(uint8_t reg, uint8_t imm)
{
    v[reg] = (uint8_t)random(mt) & imm;
}

// draw sprite (idk, this might work)
// it probably doesn't wrap correctly though

void chip8::op_dxyn(uint8_t regA, uint8_t regB, uint8_t imm)
{
    uint8_t xPos = v[regA];
    uint8_t yPos = v[regB];
    uint8_t spriteHeight = imm;

    int i,j;
    int vramIndex;

    //std::cout << std::dec << "Sprite initial position, height: " << "(" << (int)xPos << ", " << (int)yPos;
    //std::cout << "), " << (int)spriteHeight << std::endl;

    for (i=0;i<spriteHeight;++i) {
        for (j=0;j<8;++j) {
            vramIndex = 64*((yPos + i) % 32) + ((xPos+j) % 64);

            if ((bool)display[vramIndex] && (bool)(mem[index+i] & (0x80 >> j))) {
                v[0xf] |= 0x01;
            }

            display[vramIndex] ^= (mem[index+i] & (0x80 >> j));
        }
    }
    drawFlag |= 0x01;
}


// skip if key in vx is pressed
void chip8::op_ex9e(uint8_t reg)
{
    //std::cout << "ex9e called" << std::endl;
    if ( keypad & (0x01 << v[reg]) )
        pc += 2;
}

// skip if key in vx not pressed
void chip8::op_exa1(uint8_t reg)
{
    //std::cout << "exa1 called" << std::endl;
    if (!(keypad & (0x01 << v[reg])) ) {
        pc+=2;
    }
}

// reg = delay timer value
void chip8::op_fx07(uint8_t reg)
{
    v[reg] = delay;
}

// wait for keypad input, then store key val in vx
void chip8::op_fx0a(uint8_t reg)
{
    if (!keypad) {
        pc -=2;
    }

    for (int i=0;i<16;++i) {
        if (keypad & (0x01 << i))
            v[reg] = i;
    }
}

// set delay timer 
void chip8::op_fx15(uint8_t reg)
{
    delay = v[reg];
}

// set sound timer
void chip8::op_fx18(uint8_t reg)
{
    sound = v[reg];
}

// increment index address
void chip8::op_fx1e(uint8_t reg)
{
    index += v[reg];
}

// set index to font address
void chip8::op_fx29(uint8_t reg)
{
    index = 0x50 + (v[reg] * 5);
}

// BCD of vx into index[0..2]
void chip8::op_fx33(uint8_t reg)
{
    int hundreds, tens, ones, num;

    num = v[reg];

    hundreds = num / 100;
    num -= 100*hundreds;

    tens = num / 10;
    ones = num % 10;

    mem[index] = hundreds;
    mem[index+1] = tens;
    mem[index+2] = ones;
}

// dump registers v0 - vx
void chip8::op_fx55(uint8_t reg)
{
    for (int i=0x0;i<=reg;++i) {
        mem[index++] = v[i];
    }
}

// read registers v0 - vx from index
void chip8::op_fx65(uint8_t reg)
{
    for (int i=0x0;i<=reg;++i) {
        v[i] = mem[index++];
    }
}