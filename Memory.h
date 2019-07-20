//
// Created by aiden on 15/07/19.
//

#ifndef CHIP8CPP_MEMORY_H
#define CHIP8CPP_MEMORY_H

#include <cstdint>
#include <cstdio>
#include <cassert>
#include <string>

class Memory {
public:
    uint8_t memory[4096];

    void loadRom(std::string path) {
        FILE *file = fopen(path.c_str(), "rb");
        int curr = 0x200;

        while (file != nullptr) {
            unsigned int c = getc(file);
            if (c == EOF) {
                break;
            }
            assert(c < (1 << 8));
            memory[curr] = c;
            curr++;
        }
        fclose(file);
    }

    uint16_t loadWord(uint16_t ptr) {
        return memory[ptr] << 8 | memory[ptr + 1];
    }

    void dprint() {

    }

    void init() {

    }
};



#endif //CHIP8CPP_MEMORY_H
