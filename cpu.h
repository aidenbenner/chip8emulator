//
// Created by aiden on 15/07/19.
//

#ifndef CHIP8CPP_CPU_H
#define CHIP8CPP_CPU_H


#include <sstream>
#include <chrono>
#include "Memory.h"
#include "macros.h"

class Cpu {

public:
    Cpu(Memory *memory_) : ram(memory_) { }

    uint16_t gfx[64 * 32];
    uint16_t key[16];
private:
    Memory *ram;
    uint8_t V[16];
    uint16_t I;
    uint16_t pc;

    uint16_t stack[16];
    uint16_t sp;

    uint16_t opcode;

    uint16_t delay_timer;
    uint16_t sound_timer;
    uint64_t last_time_tick_time_ms;

public:
    bool drawFlag = true;

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




public:
    void init() {
        pc = 0x200;
        opcode = 0;
        I = 0;
        sp = 0;

        for (int i = 0; i<16; i++) {
            V[i] = 0;
            key[i] = 0;
        }

        for (int i = 0; i<80; i++) {
            ram->memory[i] = chip8_fontset[i];
        }

        // TODO: fontset
        D("CPU INIT");
    }

    void not_implemented() {
        D("WARNING - Instruction not implemented ?? 0x" << std::hex << pc << " 0x" << std::hex << opcode);
        // assert(false);
    }

    std::string vstr(uint8_t X) {
        return std::string("V[") + hstr(X) + "]";
    }

    std::string hstr(int X) {
        std::stringstream stream;
        stream << "0x" << std::hex << X;

        return stream.str();
    }


    std::string debug_str() {
        std::string reg;
        reg += "I = " + hstr(I) + "  PC = " + hstr(pc);
        for (int i = 0; i<16; i++) {
            reg += std::string("\t") + vstr(i) + " = " + hstr(V[i]) + "\n";
        }
        return reg;
    }

    std::string disassemble(uint16_t opcode) {
        uint16_t NNN = opcode & 0x0FFF;
        uint8_t NN = opcode & 0x00FF;
        uint8_t N = opcode & 0x000F;
        uint8_t X = (opcode & 0x0F00) >> 8;
        uint8_t Y = (opcode & 0x00F0) >> 4;


        std::string instr;
        switch(opcode & 0xF000) {
            case 0x0000:
                switch (opcode) {
                    case 0x00E0:
                        instr = "CLS";
                        break;
                    case 0x00EE:
                        instr = "RET";
                        break;
                    default:
                        not_implemented();
                }
                break;
            case 0x1000:
                instr = "JP " + hstr(NNN);
                break;
            case 0x2000:
                instr = "CALL " + hstr(NNN);
                break;
            case 0x3000:
                instr = std::string("SE V[") + hstr(X) + "] " + hstr(NN);
                break;
            case 0x4000:
                instr = std::string("SNE V[") + hstr(X) + "] " + hstr(NN);
                break;
            case 0x5000:
                instr = std::string("SE ") + vstr(X) + " " + vstr(Y);
                break;
            case 0x6000:
                instr = std::string("LD ") + vstr(X) + " " + hstr(NN);
                break;
            case 0x7000:
                instr = std::string("ADD ") + vstr(X) + " " + hstr(NN);
                break;
            case 0x8000:
                switch (opcode & 0x000F) {
                    case 0x0:
                        instr = std::string("LD ") + vstr(X) + " " + vstr(Y);
                        break;
                    case 0x1:
                        instr = std::string("OR ") + vstr(X) + " " + vstr(Y);
                        break;
                    case 0x2:
                        instr = std::string("AND ") + vstr(X) + " " + vstr(Y);
                        break;
                    case 0x3:
                        instr = std::string("XOR ") + vstr(X) + " " + vstr(Y);
                        break;
                    case 0x4:
                        instr = std::string("AND ") + vstr(X) + " " + vstr(Y);
                        break;
                    case 0x5:
                        instr = std::string("SUB ") + vstr(X) + " " + vstr(Y);
                        break;
                    case 0x6:
                        instr = std::string("SHR ") + vstr(X) + " {, " + vstr(Y);
                        break;
                    case 0x7:
                        instr = std::string("SUBN ") + vstr(X) + " " + vstr(Y);
                        break;
                    case 0xE:
                        instr = std::string("SHL ") + vstr(X) + " {," + vstr(Y);
                        break;
                    default:
                        not_implemented();
                }
                break;

            case 0x9000:
                switch(opcode & 0x000F) {
                    case 0:
                        instr = std::string("SNE ") + vstr(X) + " " + vstr(Y);
                        break;
                    default:
                        not_implemented();
                }
                break;
            case 0xA000:
                instr = std::string("LD I = ") + hstr(NNN);
                break;
            case 0xB000:
                instr = std::string("JMP V[0] ") + hstr(NN);
                break;
            case 0xC000:
                instr = std::string("RND ") + vstr(X) + std::string(" ") + hstr(NN);
                break;
            case 0xD000:
                instr = std::string("DRW ") + vstr(X) + " " + vstr(Y) + " " + hstr(N);
                break;
            case 0xE000:
                switch (opcode & 0xFF) {
                    case 0x9E:
                        instr = std::string("SKP ") + vstr(X);
                        break;
                    case 0xA1:
                        instr = std::string("SKNP ") + vstr(X);
                        break;
                    default:
                        not_implemented();
                }
                break;
            case 0xF000:
                switch (opcode & 0xFF) {
                    case 0x07:
                        instr = std::string("LDdl ") + vstr(X) + " " +  hstr(delay_timer);
                        break;
                    case 0x0A:
                        instr = std::string("LDkey ") + vstr(X);
                        break;
                    case 0x15:
                        break;
                    case 0x18:
                        break;
                    case 0x1E:
                        break;
                    case 0x29:
                        break;
                    case 0x33: {
                        break;
                    }
                    case 0x55: {
                        instr = "LD I";
                        break;
                    }
                    case 0x65: {
                        instr = "STORE I";
                        break;
                    }
                    default:
                        not_implemented();
                }
                break;
            default:
                not_implemented();
        }
        return instr;
    }

    void cycle() {
        opcode = ram->loadWord(pc);

        // D("CYCLE 0x" << std::hex << pc << " 0x" << opcode << " ");
        D(disassemble(opcode));
        ///D(debug_str());
        pc += 2;

        uint16_t NNN = opcode & 0x0FFF;
        uint8_t NN = opcode & 0x00FF;
        uint8_t N = opcode & 0x000F;
        uint8_t X = (opcode & 0x0F00) >> 8;
        uint8_t Y = (opcode & 0x00F0) >> 4;

        switch(opcode & 0xF000) {
            case 0x0000:
                switch (opcode) {
                    case 0x00E0:
                        for (int i = 0; i<64 * 32; i++) {
                            gfx[i] = 0;
                        }
                        drawFlag = true;
                        break;
                    case 0x00EE:
                        // return
                        pc = stack[sp];
                        sp--;
                        break;
                    default:
                        not_implemented();
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
                stack[sp] = pc;
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
                if (V[X] == V[Y]) {
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
                        if (((int)V[X] + V[Y]) > 0xFF) {
                            V[0xF] = 1;
                        } else {
                            V[0xF] = 0;
                        }
                        V[X] += V[Y];
                        break;
                    case 0x5:
                        if (V[X] > V[Y]) {
                            V[0xF] = 1;
                        } else {
                            V[0xF] = 0;
                        }
                        V[X] -= V[Y];
                        break;
                    case 0x6:
                        V[0xF] = 0x1 & V[X];
                        V[X] >>= 1;
                        break;
                    case 0x7:
                        if (V[X] < V[Y]) {
                            V[0xF] = 1;
                        } else {
                            V[0xF] = 0;
                        }
                        V[X] = V[Y] - V[X];
                        break;
                    case 0xE:
                        // idk if this is a bool?
                        V[0xF] = (V[X] & 0x80) != 0;
                        V[X] <<= 1;
                        break;
                    default:
                        not_implemented();
                }
                break;

            case 0x9000:
                switch(opcode & 0x000F) {
                    case 0:
                        if (V[X] != V[Y]) {
                            pc += 2;
                        }
                        break;
                    default:
                        not_implemented();
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
                V[X] = (rand() % 256) & NN;
                break;
            case 0xD000:
                V[0xF] = 0;
                for (int y = 0; y<N; y++) {
                    unsigned short pixel = ram->memory[I + y];
                    int offset = (y + V[Y]) * 64 + V[X];
                    for (int x = 0; x<8; x++) {
                        if ((pixel & (0x80 >> x)) != 0) {
                            int ind = offset + x;
                            if (V[0xF] | gfx[ind] == 1) {
                                V[0xF] = 1;
                            }
                            gfx[ind] ^= 1;
                        }
                    }
                }
                drawFlag = true;

                // D("DRAW " << V[X] << " " << V[Y] << " " << N << " " << I)
                break;
            case 0xE000:
                switch (opcode & 0xFF) {
                    case 0x9E:
                        if (key[V[X]]) {
                            pc += 2;
                        }
                        break;
                    case 0xA1:
                        if (!key[V[X]]) {
                            pc += 2;
                        }
                        break;
                    default:
                        not_implemented();
                }
                break;
            case 0xF000:
                switch (opcode & 0xFF) {
                    case 0x07:
                        V[X] = delay_timer;
                        break;
                    case 0x0A: {
                        int k = -1;
                        while (k == -1) {
                            for (int i = 0; i<16; i++) {
                                if (key[i]) {
                                    k = i;
                                }
                            }
                        }
                        V[X] = k;
                        break;
                    }
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
                        I = V[X] * 5;
                        // D(" I = " << V[X])
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
                        I = I + X + 1;
                        break;
                    }
                    case 0x65: {
                        for (uint16_t i = 0; i<=X; i++) {
                            V[i] = ram->memory[I + i];
                        }
                        I = I + X + 1;
                        break;
                    }
                    default:
                        not_implemented();
                }
                break;
            default:
                not_implemented();
        }

        uint64_t curr_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        // time ticks at 60hz always
        if (curr_time_ms - last_time_tick_time_ms > 16.666) {
            last_time_tick_time_ms = curr_time_ms;

            if (delay_timer > 0)
                delay_timer--;

            if (sound_timer > 0)
            {
                if (sound_timer == 1) {
                    // TODO: beep
                }
                sound_timer--;
            }
        }


    }
};


#endif //CHIP8CPP_CPU_H
