# A Chip 8 Emulator
Written in C++

More info about CHIP8: https://en.wikipedia.org/wiki/CHIP-8

Trace from disassembled cpu instructions running pong.
```
CPU INIT
CALL 0x2fc
LD V[0xb] 0x20
LD V[0xc] 0x0
LD I = 0x2f6
DRW V[0xb] V[0xc] 0x4
ADD V[0xc] 0x4
SE V[0xc] 0x20
JP 0x302
DRW V[0xb] V[0xc] 0x4
ADD V[0xc] 0x4
SE V[0xc] 0x20
JP 0x302
DRW V[0xb] V[0xc] 0x4
ADD V[0xc] 0x4
SE V[0xc] 0x20
JP 0x302
DRW V[0xb] V[0xc] 0x4
ADD V[0xc] 0x4
SE V[0xc] 0x20
JP 0x302
DRW V[0xb] V[0xc] 0x4
ADD V[0xc] 0x4
SE V[0xc] 0x20
...
```


## Demo:

Space Invaders
![](https://i.imgur.com/F4sX9uC.gif)

Tetris
![](https://i.imgur.com/bLzBbAA.gif)

Blinky
![](https://i.imgur.com/UCjROpe.png)

Pong
![](https://i.imgur.com/rAEET4c.png)


