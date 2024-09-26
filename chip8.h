#ifndef CHIP_8
#define CHIP_8

class chip8 {
public:
    uint8_t display[64*32];
    uint16_t keypad;

    bool audioIsPlaying();

private:
    uint8_t  v[16];
    uint16_t pc, index;

    uint16_t stack[16];
    uint8_t  sp;

    uint8_t delay, sound;

    uint8_t mem[4096];

    bool decrementTimers;

public:
    void reset();
    void loadProgram(char const* filename);
    void runCycle();
    void decrementTimers();
    
private:
    void executeInstruction(uint16_t instr);
    void op_00e0();
    void op_00ee();
    void op_1nnn(uint16_t addr);
    void op_2nnn(uint16_t addr);
    void op_3xkk(uint8_t reg, uint8_t imm);
    void op_4xkk(uint8_t reg, uint8_t imm);
    void op_5xy0(uint8_t regA, uint8_t regB);
    void op_6xkk(uint8_t reg, uint8_t imm);
    void op_7xkk(uint8_t reg, uint8_t imm);
    void op_8xy0(uint8_t regA, uint8_t regB);
    void op_8xy1(uint8_t regA, uint8_t regB);
    void op_8xy2(uint8_t regA, uint8_t regB);
    void op_8xy3(uint8_t regA, uint8_t regB);
    void op_8xy4(uint8_t regA, uint8_t regB);
    void op_8xy5(uint8_t regA, uint8_t regB);
    void op_8xy6(uint8_t regA, uint8_t regB);
    void op_8xy7(uint8_t regA, uint8_t regB);
    void op_8xye(uint8_t regA, uint8_t regB);
    void op_9xy0(uint8_t regA, uint8_t regB);
    void op_annn(uint16_t addr);
    void op_bnnn(uint16_t addr);
    void op_cxkk(uint8_t reg, uint8_t imm);
    void op_dxyn(uint8_t regA, uint8_t regB, uint8_t imm);
    void op_ex9e(uint8_t reg);
    void op_exa1(uint8_t reg);
    void op_fx07(uint8_t reg);
    void op_fx0a(uint8_t reg);
    void op_fx15(uint8_t reg);
    void op_fx18(uint8_t reg);
    void op_fx1e(uint8_t reg);
    void op_fx29(uint8_t reg);
    void op_fx33(uint8_t reg);
    void op_fx55(uint8_t reg);
    void op_fx65(uint8_t reg);
};

#endif
