//
// Created by aiden on 15/07/19.
//

#ifndef CHIP8CPP_MACROS_H
#define CHIP8CPP_MACROS_H

#include <iostream>
#define DEBUG 1

#ifdef DEBUG
#  define D(x) std::cerr << x << std::endl;
#else
#  define D(x)
#endif // DEBUG


#endif //CHIP8CPP_MACROS_H
