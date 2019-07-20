#include <iostream>
#include <zconf.h>
#include "chip8.h"
#include "SDL2/SDL.h"

int main() {
    Chip8 chip8;

    chip8.init();
    chip8.loadRom("data/tetris.rom");

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

        usleep(1 * 10 * 1000);

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

        chip8.setKeys();
    }

    return 0;
}