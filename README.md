# CHIP-8 emulator

Chip8 Emulator written in C++ for learning purposes

## About CHIP-8

CHIP-8 is an interpreted programming language developed in the 1970s for simple video games. This emulator implements the CHIP-8 instruction set and can run classic CHIP-8 programs.

## Features

- Full CHIP-8 instruction set implementation
- Graphics display (64x32 monochrome)
- Keyboard input support
- Adjustable clock speed
- Cross-platform compatibility

## Prerequisites

- CMake 3.23 or higher
- C++20 compatible compiler

## Building

1. Clone the repository
    ```bash
    git clone https://github.com/Tomb909/chip8-emulator.git
    cd chip8-emulator
    ```
    
2. Create build directory and generate makefiles:
    ```bash
    cmake -B build
    ```

3. Build the project:
    ```bash
    cmake --build build
    ```

## Usage

Run the emulator with:
```bash
./build/src/chip8 <rom_file> [clock_speed]
```

![IBM Logo Test](/IBMtest.png)
![Tetris Test](/TetrisTest.png)

## Things Left to Implement

- Audio 

## Acknowledgements

- [multigesture.net Chip8 Article](https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)
- [Cowgod's Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#font)
- [Chip-8 Variant Opcode Table](https://chip8.gulrak.net/#quirk1)
- [Chip-8 Wikipedia Page](https://en.wikipedia.org/wiki/CHIP-8)