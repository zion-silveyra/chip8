#include "chip8.h"
#include <random>
#include <fstream>
#include <array>

void chip8::reset()
{
    std::fill(std::begin(display), std::end(display), 0);
    std::fill(std::begin(v), std::end(v), 0);
    pc = 0x200;
    sp = 0;
    delay = 0;
    sound = 0;
}

void chip8::loadProgram(char const* filename)
{
    std::ifstream ifstream;
    ifstream.open(filename, std::ios::binary);

    char buf[0x1000-0x200];
    ifstream.read(buf, 0x1000-0x200);

    std::copy(std::begin(buf), std::end(buf), &mem[0x200]);
}

void chip8::runCycle()
{
    // read instruction from pc, pc += 2
    // determine instruction and arguments
    // call function
    // if ext timer-- flag set, do so   

    if (decrementTimers) {
        --delay;
        --sound;
    }
    
    uint16_t instr = (mem[pc] << 8) | mem[pc+1];
    pc += 2;

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
        if (instr & 0x00FF == 0x9e) op_ex9e(regX);
        if (instr & 0x00FF == 0xa1) op_exa1(regX);
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

// Instruction set as specified in //
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM // 

// Clear display
void chip8::op_00e0()
{
    std::fill(std::begin(display), std::end(display), 0);
}

// Return from subroutine
void chip8::op_00ee()
{
    pc = mem[sp];
    --sp;
}

// jump
void chip8::op_1nnn(uint16_t addr)
{
    pc = addr;
}

// call subroutine 
void chip8::op_2nnn(uint16_t addr)
{
    ++sp;
    stack[sp] = pc;
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
}

// vx = vx | vy
void chip8::op_8xy1(uint8_t regA, uint8_t regB)
{
    v[regA] |= v[regB];
}

// vx = vx & vy
void chip8::op_8xy2(uint8_t regA, uint8_t regB)
{
    v[regA] &= v[regB];
}

// xor
void chip8::op_8xy3(uint8_t regA, uint8_t regB)
{
    v[regA] ^= v[regB];
}

// add
void chip8::op_8xy4(uint8_t regA, uint8_t regB)
{
    uint16_t wide_result{ v[regA] + v[regB]};
    uint8_t  result{ v[regA] + v[regB]};

    if (wide_result != result) v[0xf] = 1;
    
    v[regA] = result;
}

// sub
void chip8::op_8xy5(uint8_t regA, uint8_t regB)
{
    if (v[regA] < v[regB]) v[0xf] = 1;
    v[regA] -= v[regB];
}

// shift right
void chip8::op_8xy6(uint8_t regA, uint8_t regB)
{
    // regB is ignored ?? ehh

    if (v[regA] & 0x01)
        v[0xf] = 1;
    
    v[regA] = v[regA] >> 1;
}

// subn
void chip8::op_8xy7(uint8_t regA, uint8_t regB)
{
    if (v[regB] < v[regA])
        v[0xf] = 1;

    v[regA] = v[regB] - v[regA];
}

// shift left
void chip8::op_8xye(uint8_t regA, uint8_t regB)
{
    // regB ignored

    if (v[regA] & 0x80)
        v[0xf] = 1;

    v[regA] = v[regA] << 1;
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
    i = addr;
}

// jump offset (v0 + addr)
void chip8::op_bnnn(uint16_t addr)
{
    pc = v[0] + addr;
}

// random number, then AND 
void chip8::op_cxkk(uint8_t reg, uint8_t imm)
{
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0x00, 0xff);

    v[reg] = distribution(generator) & imm;
}

// draw sprite 
void chip8::op_dxyn(uint8_t regA, uint8_t regB, uint8_t imm)
{

}
void chip8::op_ex9e(uint8_t reg)
{

}
void chip8::op_exa1(uint8_t reg)
{

}
void chip8::op_fx07(uint8_t reg)
{

}
void chip8::op_fx0a(uint8_t reg)
{

}
void chip8::op_fx15(uint8_t reg)
{

}
void chip8::op_fx18(uint8_t reg)
{

}
void chip8::op_fx1e(uint8_t reg)
{

}
void chip8::op_fx29(uint8_t reg)
{

}
void chip8::op_fx33(uint8_t reg)
{

}
void chip8::op_fx55(uint8_t reg)
{

}
void chip8::op_fx65(uint8_t reg)
{

}