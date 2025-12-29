#ifndef CHIP8_H
#define CHIP8_H

#include <string>

class chip8
{
private:
    uint16_t opcode; // current opcode (35 opcodes, each 2 bytes) uint16_t = 2 bytes
    uint8_t memory[4096];
    
    // memory map
    // 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
    // 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
    // 0x200-0xFFF - Program ROM and work RAM
    
    uint8_t v[16]; // 15 8 bit general purpose registers and 16th for 'carry flag'
    uint16_t I; // index register 
    uint16_t pc; // program counter
    uint8_t gfc[64 * 32]; // graphics for chip 8 system, 64x32 screen (2048 pixels)

    // timer registers count at 60Hz, when above 0 they count down to 0
    uint8_t delay_timer;
    uint8_t sound_timer;

    uint16_t stack[16];
    uint16_t sp; // stack pointer

    uint8_t key[16]; // hex based key pad, stores current state of key

public:
    void initialise();
    void loadGame(std::string gameName);
    void emulateCycle();
    bool drawFlag();
    void setKeys();


};

void initialiseGraphics();
void initialiseInput();

#endif