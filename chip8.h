#ifndef CHIP8_H
#define CHIP8_H

#include <string>

class chip8
{
private:
    unsigned short opcode; // current opcode (35 opcodes, each 2 bytes) unsigned short = 2 bytes
    unsigned char memory[4096];
    
    // memory map
    // 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
    // 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
    // 0x200-0xFFF - Program ROM and work RAM
    
    unsigned char v[16]; // 15 8 bit general purpose registers and 16th for 'carry flag'
    unsigned short I; // index register 
    unsigned short pc; // program counter
    unsigned char gfc[64 * 32]; // graphics for chip 8 system, 64x32 screen (2048 pixels)

    // timer registers count at 60Hz, when above 0 they count down to 0
    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned short stack[16];
    unsigned short sp; // stack pointer

    unsigned char key[16]; // hex based key pad, stores current state of key

public:
    void initialise();
    void loadGame(std::string gameName);
    void emulateCycle();
    bool drawFlag();
    void setKeys();


};

#endif