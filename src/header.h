#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <Windows.h>

#define UNICODE

typedef struct Chip8State {
    uint8_t V[16];
    uint16_t I;
    uint16_t SP;
    uint16_t PC;
    uint8_t delay;
    uint8_t sound;
    uint8_t *memory;
    uint8_t *screen; // starts at 0xFF00
    uint8_t key_press;
} Chip8State;


void disassemble(uint8_t *, int);
void emulate(Chip8State *);
Chip8State* InitChip8(void);

