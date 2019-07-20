//
// Created by aiden on 15/07/19.
//

#ifndef CHIP8CPP_CHIP8_H
#define CHIP8CPP_CHIP8_H

#include <string>
#include <cstdio>
#include <cassert>
#include "cpu.h"
#include "memory.h"


class Chip8 {
    Memory *memory;


public:
    Chip8() : memory(new Memory), cpu(memory) {
    }

    ~Chip8() {
        delete memory;
    }

    void cycle() {
        cpu.cycle();
    }

    void draw() {

    }

    void setKeys() {

    }

    bool drawFlag;

    void loadRom(std::string path) {
        memory->loadRom(path);
    }

    void init() {
        memory->init();
        cpu.init();
    }

    Cpu cpu;
};

#endif //CHIP8CPP_CHIP8_H
