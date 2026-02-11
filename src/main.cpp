#include "SDL3/SDL_events.h"
#include "SDL3/SDL_timer.h"
#include "chip8.h"
#include <SDL3/SDL.h>
#include <map>

chip8 Chip8;
static int current_sine_sample = 0;

int main(int argc, char **argv) {
    Chip8.initialiseGraphics();
    // initialiseInput();

    int running = 1;
    SDL_Event e;
    int clockSpeed = 1000; // in Hz

    
    Chip8.initialise();
    
    if (argc < 2) {
        printf("Usage: ./chip8 gameROMName <clockSpeedHz>\n");
        return 1;
    }
    
    Chip8.loadGame(argv[1]);

    if (argc > 2) {
        clockSpeed = std::stoi(argv[2]);
    }

    std::map<SDL_Keycode, uint8_t> keyMap = {{SDLK_1, 1}, {SDLK_2, 2}, {SDLK_3, 3}, {SDLK_4, 0xC},
                                                 {SDLK_Q, 4}, {SDLK_W, 5}, {SDLK_E, 6}, {SDLK_R, 0xD},
                                                 {SDLK_A, 7}, {SDLK_S, 8}, {SDLK_D, 9}, {SDLK_F, 0xE},
                                                 {SDLK_Z, 0xA}, {SDLK_X, 0}, {SDLK_C, 0xB}, {SDLK_V, 0xF}};

    auto lastTimerUpdate = SDL_GetTicks();
    
    while (running) {

        auto cycleStart = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = 0;
            }
            else if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.key == SDLK_ESCAPE) running = 0;
                else if (keyMap.find(e.key.key) != keyMap.end()) Chip8.setKey(keyMap[e.key.key], true);
            }
            else if (e.type == SDL_EVENT_KEY_UP) {
                if (keyMap.find(e.key.key) != keyMap.end()) Chip8.setKey(keyMap[e.key.key], false);
            }
        }

        if (SDL_GetTicks() - lastTimerUpdate > 16) {
            Chip8.tickTimers();
            lastTimerUpdate = SDL_GetTicks();
        }

        Chip8.emulateCycle();

        auto cycleEnd = SDL_GetTicks();
        if ((cycleEnd - cycleStart) < (1000 / clockSpeed)) {
            SDL_Delay(1000 / clockSpeed - (cycleEnd - cycleStart));
        }

    }

    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();

    return 0;
}

