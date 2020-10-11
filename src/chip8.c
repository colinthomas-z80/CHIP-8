#include "header.h"

static void push(Chip8State *, uint16_t);
static uint16_t pop(Chip8State *);

Chip8State* InitChip8(void)
{
    Chip8State* s = calloc(sizeof(Chip8State), 1);

    s->memory = calloc(1024*4, 1);
    s->screen = &s->memory[0xf00];
    s->SP = 0xfa0;
    s->PC = 0x200;

    return s;
}

void emulate(Chip8State *machine)
{
    uint8_t *code = &machine->memory[machine->PC];

    int highnib = (*code & 0xf0) >> 4;
    switch(highnib)
    {
        case 0x00: break;
        case 0x01: machine->PC = (code[0]&0x0f<<8) | code[1]; break;
        case 0x02: 
            {
                // An experimental stack cleanup
                push(&machine, (code[0]&0x0f<<8) | code[1]);
                // machine->SP -= 2;                                     
                // machine->memory[machine->SP] = ((machine->PC+2) & 0xFF00) >> 8;
                // machine->memory[machine->SP+1] = (machine->PC+2) & 0xFF;
                // machine->PC = ((code[0]&0xf)<<8) | code[1]; 
            }; break;
        case 0x03:
            {
                uint8_t reg = code[0] & 0x0f;
                if(machine->V[reg] == code[1])
                    machine->PC += 2;
                state->PC+=2;
            } 
    }

}

void push(Chip8State *machine, uint16_t code)
{
    machine->SP -= 2;
    machine->memory[machine->SP] = ((machine->PC+2) & 0xFF00) >> 8;
    machine->memory[machine->SP+1] = (machine->PC+2) & 0xFF;
    machine->PC = code;
}