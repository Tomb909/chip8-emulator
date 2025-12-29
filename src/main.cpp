#include "chip8.h"

chip8 Chip8;

int main(int argc, char **argv) {

    Chip8.initialise();

    if (argc < 2) {
        printf("Usage: ./chip8 gameFileName\n");
        return 1;
    }

    Chip8.loadGame(argv[1]);
    
    return 0;
}