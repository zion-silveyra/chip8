class chip8 {
public:
    uint8_t display[64 * 32];
    uint16_t keypad;

    bool audioIsPlaying();

private:
    uint8_t reg[16];
    uint16_t pc, i;

    uint16_t stack[16];
    uint8_t  sp;

    uint8_t delay, sound;

    uint8_t mem[4096];

public:
    void init();
    void reset();
    bool loadProgram(char const* filename);
    void runCycle();
    
private:
    void 00e0();
    void 00ee();
    void 1nnn(uint16_t addr);
    void 2nnn(uint16_t addr);
    void 3xkk(uint8_t reg, uint8_t imm);
    void 4xkk(uint8_t reg, uint8_t imm);
    void 5xy0(uint8_t regA, regB);
    void 6xkk(uint8_t reg, uint8_t imm);
    void 7xkk(uint8_t reg, uint8_t imm);
    void 8xy0(uint8_t regA, uint8_t regB);
    void 8xy1(uint8_t regA, uint8_t regB);
    void 8xy2(uint8_t regA, uint8_t regB);
    void 8xy3(uint8_t regA, uint8_t regB);
    void 8xy4(uint8_t regA, uint8_t regB);
    void 8xy5(uint8_t regA, uint8_t regB);
    void 8xy6(uint8_t regA, uint8_t regB);
    void 8xy7(uint8_t regA, uint8_t regB);
    void 8xye(uint8_t regA, uint8_t regB);
    void 9xy0(uint8_t regA, uint8_t regB);
    void annn(uint16_t addr);
    void bnnn(uint16_t addr);
    void cxkk(uint8_t reg, uint8_t imm)
    void dxyn(uint8_t regA, uint8_t regB, uint8_t imm)
    void ex9e(uint8_t reg);
    void exa1(uint8_t reg);
    void fx07(uint8_t reg);
    void fx0a(uint8_t reg);
    void fx15(uint8_t reg);
    void fx18(uint8_t reg);
    void fx1e(uint8_t reg);
    void fx29(uint8_t reg);
    void fx33(uint8_t reg);
    void fx55(uint8_t reg);
    void fx65(uint8_t reg);
};