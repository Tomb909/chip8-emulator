#include "chip8.h"
#include <fstream>
#include <iostream>

unsigned char chip8_fontset[80] =
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

void chip8::initialise() {
    // initialise registers and memory 
    opcode = 0;
    pc = 0x200;
    I = 0; //clear index register
    sp = 0;

    std::fill_n(stack, 16, 0); // clear stack
    std::fill_n(gfc, 2048, 0); // clear display
    std::fill_n(v, 16, 0); // clear registers
    std::fill_n(memory, 4096, 0); // clear memory

    // load fontset into first 80 bytes of memory
    for (int i = 0; i < 80; i++) {
        memory[i] = chip8_fontset[i];
    }

    delay_timer = 0;
    sound_timer = 0;
}

void chip8::loadGame(std::string gameName) {
    std::ifstream file( (gameName + ".ch8").c_str(), std::ios::binary);

    if (!file) throw std::runtime_error("Game ROM file could not be opened\n");

    // get length of file
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();

    // memory size is 4096, ROM stored at 512 onwards, 4096 - 512 = 3854
    if (size > 3854) {
        throw std::runtime_error("Game ROM is too large to be stored in memory");
    }

    // move seek get pointer back to start
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(&memory[512]), size);
}