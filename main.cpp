#include <iostream>
#include <zconf.h>
#include "chip8.h"
#include "SDL2/SDL.h"

int main() {
    Chip8 chip8;

    chip8.init();
    chip8.loadRom("data/invaders.rom");

    SDL_Window* window = NULL;

    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
        exit(1);
    }

    int width = 1000;
    int height = 500;
    window = SDL_CreateWindow(
            "CHIP-8 CPP",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height, SDL_WINDOW_SHOWN
    );

    if (window == NULL){
        exit(2);
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, width, height);
    SDL_Texture* sdlTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
    uint32_t pixels[2048];

    while (true) {
        chip8.cycle();
        SDL_Event event;

        // 60hz = 1/60s = 1.66666ms
        usleep(1 * 1 * 500);

        if (chip8.cpu.drawFlag) {
            chip8.cpu.drawFlag = false;

            for (int i = 0; i < 2048; ++i) {
                uint8_t pixel = chip8.cpu.gfx[i];
                pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
            }
            SDL_UpdateTexture(sdlTexture, NULL, pixels, 64 * sizeof(Uint32));
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }

        auto setKey = [&](char key, int val) {
            if (isdigit(key)) {
                chip8.cpu.key[key - '0'] = val;
            }
            else if (isalpha(key)) {
                if (key > 'f' || key < 'a') {

                } else {
                    chip8.cpu.key[10 + key - 'a'] = val;
                }
            }
        };

        while (SDL_PollEvent(&event)) {
            char key = event.key.keysym.sym;
            switch(event.type) {
                case SDL_KEYDOWN:
                    setKey(key, 1);
                    break;
                case SDL_KEYUP:
                    setKey(key, 0);
                    break;
            }
        }

        chip8.setKeys();
    }

    return 0;
}