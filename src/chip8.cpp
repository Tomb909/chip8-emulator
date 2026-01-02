#include "chip8.h"
#include <fstream>
#include <iostream>
#include <random>

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

    delayTimer = 0;
    soundTimer = 0;
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

void chip8::emulateCycle() {
    // fetch opcode at pc, (one opcode is 2 bytes long so we use a trick to merge the bytes)
    opcode = memory[pc] << 8 | memory[pc + 1];

    // decode and execute opcode:
    uint16_t nnn = opcode & 0x0FFF;
    uint8_t n = opcode & 0x000F;
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t kk = opcode & 0x00FF;

    switch (opcode & 0xF000) 
    {
        case 0x0000:
        switch (opcode & 0x000F) {
            case 0x0000:
                std::fill_n(gfc, 2048, 0); // clear display
                updateScreen = true;
                pc += 2;
            break;
            case 0x000E:
                pc = stack[--sp];
                pc += 2;
            break;
            default:
                std::cout << "Unknown opcode: " << opcode << '\n';
        }
        break;
        case 0x1000:
            pc = nnn;
        break;
        case 0x2000:
            stack[++sp] = pc;
            pc = nnn;
        break;
        case 0x3000:
            if (v[x] == kk) {
                pc += 4;
            } else {
                pc += 2;
            }
        break;
        case 0x4000:
            if (v[x] != kk) {
                pc += 4;
            } else {
                pc += 2;
            }
        break;
        case 0x5000:
            if (v[x] == v[y]) {
                pc += 4;
            } else {
                pc += 2;
            }
        break;
        case 0x6000:
            v[x] = kk;
            pc += 2;
        break;
        case 0x7000:
            v[x] += kk;
            pc += 2;
        break;
        case 0x8000:
            switch (opcode & 0x000F)
            {
                case 0x0000:
                    v[x] = v[y];
                    pc += 2;
                break;
                case 0x0001:
                    v[x] = v[x] | v[y];
                    pc += 2;
                break;
                case 0x0002:
                    v[x] = v[x] & v[y];
                    pc += 2;
                break;
                case 0x0003:
                    v[x] = v[x] ^ v[y];
                    pc += 2;
                break;
                case 0x0004:
                    if (v[y] > (0xFF - v[x])) {
                        // overflow, so set carry
                        v[0xF] = 1;
                    } else {
                        // otherwise, there is no carry
                        v[0xF] = 0;
                    }
                    v[x] += v[y];
                    pc += 2;
                break; 
                case 0x0005:
                if (v[y] > v[x]) {
                        // underflow, so set Not borrow to 0
                        v[0xF] = 0;
                    } else {
                        // otherwise set vF, Not borrow to 1
                        v[0xF] = 1;
                    }
                    v[x] -= v[y];
                    pc += 2;
                break;
                case 0x0006:
                    v[0xF] = v[y] & 1;
                    v[x] = v[y] >> 1;
                    pc += 2;
                break;
                case 0x0007:
                    if (v[x] > v[y]) {
                        // underflow
                        v[0xF] = 0;
                    } else {
                        v[0xF] = 1;
                    }
                    v[x] = v[y] - v[x];
                break;
                case 0x000E:
                    v[0xF] = v[y] & 128;
                    v[x] = v[y] << 1;
                    pc += 2;
                break;
            default:
                std::cout << "Unknown opcode: " << opcode << '\n';
            }
        case 0x9000:
            if (v[x] != v[y]) {
                pc += 4;
            } else {
                pc += 2;
            }
        break;
        case 0xA000:
            I = nnn; 
            pc += 2;
        break;
        case 0xB000:
            pc = nnn + v[0];
        break;
        case 0xC000:
            v[x] = kk & std::uniform_int_distribution<int>(0, 255)(rng());
            pc += 2;
        break;
        case 0xD000:
            uint8_t pixelLine;

            // reset F register to used as collision register
            v[0xF] = 0;

            // each yline is a byte, opcode asks to read n bytes from memory
            for (int yLine = 0; yLine < n; y++) {
                pixelLine = memory[I + yLine];

                for (int xLine = 0; xLine < 8; xLine++) {
                    // check if single bit in line of pixels is not 0 to remove redundant work
                    if((pixelLine & (0x80 >> xLine)) != 0) {
                        // * 64 because for each y level we go we skip 64 entries     
                        if (gfc[x + xLine + ((y + yLine) * 64)] == 1) {
                            v[0xF] = 1;
                        }
                        gfc[x + xLine + ((y + yLine) * 64)] ^= 1;
                    }

                }
            }

            updateScreen = true;
            pc += 2;
            
        break;
        case 0xE000:
            switch (opcode & 0x00FF)
            {
            case 0x009E:
                if(key[v[x]] != 0) {
                    pc += 4;
                } else {
                    pc += 2;
                }
            break;
            case 0x00A1:
                if(key[v[x]] == 0) {
                    pc += 4;
                } else {
                    pc += 2;
                }
                break;
            default:
                std::cout << "Unknown opcode: " << opcode << '\n';
            }
        break;
        case 0xF000:
            switch (opcode & 0x00FF)
            {
            case 0x0007:
                v[x] = delayTimer;
                pc += 2;
            break;
            case 0x000A:
            {
                bool waitingForPress = true;

                for(int i = 0; i < 16; i++) {
                    if(key[i] != 0)
                    {
                        v[x] = i;
                        waitingForPress = false;
                    }
                }

                if(waitingForPress) {
                    return;
                }

                pc += 2;
            }
            break;
            case 0x0015:
                delayTimer = v[x];
                pc += 2;
            break;
            case 0x0018:
                soundTimer = v[x];
                pc += 2;
            break;
            case 0x001E:
                I += v[x];
                pc += 2;
            break;
            case 0x0029:
                I = v[x] * 5;
                pc += 2;
            break;
            case 0x0033:
                memory[I] = v[x] / 100;
                memory[I + 1] = (v[x] / 10) % 10;
                memory[I + 2] = v[x] % 10;
                pc += 2;
            break;
            case 0x0055:
                for (int j = 0; j <= x; j++) {
                    memory[I + j] = v[j];
                }
                I += x + 1;
                pc += 2;
            break;
            case 0x0065:
                for (int j = 0; j <= x; j++) {
                    v[j] = memory[I + j];
                }
                I += x = 1;
                pc += 2;
            break;
            default:
                std::cout << "Unknown opcode: " << opcode << '\n';
            }
        break;
        default:
            std::cout << "Unknown opcode: " << opcode << '\n';

    }
}