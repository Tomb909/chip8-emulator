#ifndef CHIP8_H
#define CHIP8_H

#include <string>
#include <random>
#include <SDL3/SDL.h>

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
    uint8_t delayTimer;
    uint8_t soundTimer;

    uint16_t stack[16];
    uint16_t sp; // stack pointer

    uint8_t key[16]; // hex based key pad, stores current state of key, 0 is not pressed

    inline std::mt19937& rng() {
        static std::mt19937 gen{std::random_device{}()};
        return gen;
    }
    void drawGraphics();

public:
    void initialise();
    void loadGame(std::string gameName);
    void emulateCycle();
    void setKey(uint8_t keyIndex, bool pressed);
    void tickTimers();

    bool updateScreen = false;
    int running = 0;
    
    void initialiseGraphics();
    SDL_Renderer *renderer;
};

void initialiseInput();

#endif