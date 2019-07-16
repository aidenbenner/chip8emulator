//
// Created by aiden on 15/07/19.
//

#ifndef CHIP8CPP_CPU_H
#define CHIP8CPP_CPU_H



#include "Memory.h"
#include "macros.h"

class Cpu {

public:
    Cpu(Memory *memory_) : ram(memory_) { }

private:
    Memory *ram;
    uint8_t V[16];
    uint16_t I;
    uint16_t pc;

    uint16_t stack[16];
    uint16_t sp;

    uint16_t key;

    uint16_t opcode;

    uint16_t delay_timer;
    uint16_t sound_timer;

    uint16_t gfx[64 * 32];

public:
    void init() {
        pc = 0x200;
        opcode = 0;
        I = 0;
        sp = 0;

        for (int i = 0; i<16; i++) {
            V[i] = 0;
        }

        // TODO: fontset
        D("CPU INIT");
    }

    void cycle() {
        opcode = ram->loadWord(pc);
        pc += 2;

        D("CPU CYCLE " << pc << " " << opcode << " ");

        uint16_t NNN = opcode & 0x0FFF;
        uint16_t NN = opcode & 0x00FF;
        uint16_t N = opcode & 0x000F;
        uint8_t X = opcode & 0x0F00;
        uint8_t Y = opcode & 0x00F0;

        switch(opcode & 0xF000) {
            case 0x0000:
                switch (opcode) {
                    case 0x00E0:
                        uint16_t gfx[64 * 32];
                        for (int i = 0; i<64 * 32; i++) {
                            gfx[i] = 0;
                        }
                        break;
                    case 0x00EE:
                        // return
                        pc = ram->memory[stack[sp]];
                        sp--;
                        break;
                }
                break;
            case 0x1000:
                // Jump to NNN
                pc = NNN;
                break;
            case 0x2000:
                // 2NNN	Flow	*(0xNNN)()	Calls subroutine at NNN.
                // todo
                sp++;
                ram->memory[stack[sp]] = pc;
                pc = NNN;
                break;
            case 0x3000:
                // 3XNN	Cond	if(Vx==NN)	Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block)
                if (V[X] == NN) {
                    pc += 2;
                }
                break;
            case 0x4000:
                // 4XNN	Cond	if(Vx!=NN)	Skips the next instruction if VX doesn't equal NN. (Usually the next instruction is a jump to skip a code block)
                if (V[X] != NN) {
                    pc += 2;
                }
                break;
            case 0x5000:
                if (V[X] != V[Y]) {
                    pc += 2;
                }
                break;
            case 0x6000:
                V[X] = NN;
                break;
            case 0x7000:
                V[X] += NN;
                break;
            case 0x8000:
                switch (opcode & 0x000F) {
                    case 0x0:
                        V[X] = V[Y];
                        break;
                    case 0x1:
                        V[X] = V[X] | V[Y];
                        break;
                    case 0x2:
                        V[X] = V[X] & V[Y];
                        break;
                    case 0x3:
                        V[X] = V[X] ^ V[Y];
                        break;
                    case 0x4:
                        V[0xF] = ((int)V[X] + V[Y]) > 0xFF;
                        V[X] += V[Y];
                        break;
                    case 0x5:
                        V[0xF] = ((int)V[X] - V[Y]) >= 0;
                        V[X] -= V[Y];
                        break;
                    case 0x6:
                        // todo: stores??
                        V[0xF] = 0x1 & V[X];
                        V[X] >>= 1;
                        break;
                    case 0x7:
                        V[0xF] = ((int)V[Y] - V[X]) >= 0;
                        V[X] = V[Y] - V[X];
                        break;
                    case 0xE:
                        // idk if this is a bool?
                        V[0xF] = V[X] & 0x80 > 0;
                        V[X] <<= 1;
                        break;
                }
                break;

            case 0x9000:
                switch(opcode & 0x000F) {
                    case 0:
                        if (V[X] != V[Y]) {
                            pc += 2;
                        }
                        break;
                }
                break;

            case 0xA000:
                I = NNN;
                break;
            case 0xB000:
                pc = V[0] + NNN;
                break;
            case 0xC000:
                // todo seed
                V[X] = rand() % 256 + NNN;
                break;
            case 0xD000:
                D("DRAW " << V[X] << " " << V[Y] << " " << N << " " << I)
                break;
            case 0xE000:
                switch (opcode & 0xFF) {
                    case 0x9E:
                        if (key == V[X]) {
                            pc += 2;
                        }
                        break;
                    case 0xA1:
                        if (key != V[X]) {
                            pc += 2;
                        }
                        break;
                }
                break;
            case 0xF000:
                switch (opcode & 0xFF) {
                    case 0x07:
                        V[X] = delay_timer;
                        break;
                    case 0x0A:
                        V[X] = key;
                        break;
                    case 0x15:
                        delay_timer = V[X];
                        break;
                    case 0x18:
                        sound_timer = V[X];
                        break;
                    case 0x1E:
                        I += V[X];
                        break;
                    case 0x29:
                        // I += V[X];
                        // todo char
                        D(" I = " << V[X])
                        break;
                    case 0x33: {
                        int dec = V[X];
                        ram->memory[I] = dec / 100;
                        ram->memory[I+1] = dec % 100 / 10;
                        ram->memory[I+2] = dec % 10;
                        break;
                    }
                    case 0x55: {
                        for (uint16_t i = 0; i<=X; i++) {
                            ram->memory[I + i] = V[i];
                        }
                        break;
                    }
                    case 0x65: {
                        for (uint16_t i = 0; i<=X; i++) {
                            V[i] = ram->memory[I + i];
                        }
                        break;
                    }
                }
                break;
        }

        if (delay_timer-- > 0)
            delay_timer--;

        if (sound_timer > 0)
        {
            if (sound_timer == 1) {
                // TODO: beep
            }
            sound_timer--;
        }
    }
};


#endif //CHIP8CPP_CPU_H
