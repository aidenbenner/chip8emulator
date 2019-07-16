#include <iostream>
#include <zconf.h>
#include "chip8.h"

int main() {
    Chip8 chip8;

    chip8.init();
    chip8.loadRom("data/tetris.rom");

    while (true) {
        chip8.cycle();

        usleep(100000);

        if (chip8.drawFlag) {
            chip8.draw();
        }

        chip8.setKeys();
    }

    return 0;
}