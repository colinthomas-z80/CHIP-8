#include "header.h"

static void push(Chip8State *);
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
                uint16_t call = (code[0]&0x0f<<8) | code[1];
                push(&machine);
                machine->PC = call;
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
                machine->PC+=2;
            }; break;
        case 0x04:
            {
                uint8_t reg = code[0] & 0x0f;
                if(machine->V[reg] != code[1])
                    machine->PC += 2;
                machine->PC+=2;
            }; break;
        case 0x05:
            {
               uint8_t reg1 = code[0] & 0x0f;
               uint8_t reg2 = code[1] >> 4;
                if(machine->V[reg1] == machine->V[reg2])
                    machine->PC += 2;
                machine->PC+=2; 
            }; break;
        case 0x06:
            {
                uint8_t reg = code[0] & 0x0f;
                machine->V[reg] = code[1];
                machine->PC += 2;
            }; break;
        case 0x07:
            {
                uint8_t reg = code[0] & 0x0f;
                machine->V[reg] += code[1];
                machine->PC += 2;
            }
        case 0x08:
            {
                switch(code[1] & 0x0f){
                    case 0x00:
                    {
                        uint8_t reg1 = code[0] & 0x0f;
                        uint8_t reg2 = code[1] >> 4;
                        machine->V[reg1] = machine->V[reg2];
                    }; break;
                    case 0x01:
                    {
                        uint8_t reg1 = code[0] & 0x0f;
                        uint8_t reg2 = code[1] >> 4;
                        machine->V[reg1] = machine->V[reg1] | machine->V[reg2];
                    }; break;
                    case 0x02:
                    {
                        uint8_t reg1 = code[0] & 0x0f;
                        uint8_t reg2 = code[1] >> 4;
                        machine->V[reg1] = machine->V[reg1] & machine->V[reg2];
                    }; break;
                    case 0x03:
                    {
                        uint8_t reg1 = code[0] & 0x0f;
                        uint8_t reg2 = code[1] >> 4;
                        machine->V[reg1] = machine->V[reg1] ^ machine->V[reg2];
                    }; break;
                    case 0x04:
                    {
                        uint8_t reg1 = code[0] & 0x0f;
                        uint8_t reg2 = code[1] >> 4;
                        uint16_t result = machine->V[reg1] = machine->V[reg2];
                        if(result & 0xff00)
                            machine->V[0x0f] = 1;
                        else
                            machine->V[0x0f] = 0;
                        machine->V[reg1] = result & 0x00ff;
                    }; break;
                    case 0x05:
                    {
                        uint8_t reg1 = code[0] & 0x0f;
                        uint8_t reg2 = code[1] >> 4;
                        int borrow = (machine->V[reg1] > machine->V[reg2]);
                        machine->V[reg1] -= machine->V[reg2];
                        machine->V[0x0f] = borrow;
                    }; break;
                    case 0x06:
                    {
                        uint8_t reg1 = code[0] & 0x0f;
                        uint8_t lsb = (reg1 << 7) >> 7;
                        machine->V[0x0f] = lsb;
                        machine->V[reg1] >> 1;
                    }; break;
                    case 0x07:
                    {
                        uint8_t reg1 = code[0] & 0x0f;
                        uint8_t reg2 = code[1] >> 4;
                        int borrow = machine->V[reg2] > machine->V[reg1];
                        machine->V[0x0f] = borrow;
                        machine->V[reg1] = machine->V[reg2] - machine->V[reg1];
                    }; break;
                    case 0x0e:
                    {
                        uint8_t reg1 = code[0] & 0x0f;
                        uint8_t msb = machine->V[reg1] >> 7;
                        machine->V[0x0f] = msb;
                        machine->V[reg1] << 1;
                    }; break;
                }
                machine->PC += 2;
            }; break;
            case 0x09:
                {
                    uint8_t reg1 = code[0] & 0x0f;
                    uint8_t reg2 = code[1] >> 4;
                    if(machine->V[reg1] != machine->V[reg2])
                        machine->PC += 2;
                    machine->PC+=2;
                }; break;
            case 0x0a:
                {
                    machine->I = ((code[0] & 0x0f) << 8) | code[1];
                    machine->PC += 2;
                }; break;
            case 0x0b:
                {
                    uint16_t offset = machine->V[0x00];
                    machine->PC = (((code[0] & 0x0f) << 8) | code[1]) + offset;
                }; break;
            case 0x0c:
                {
                   uint8_t reg1 = code[0] & 0x0f;
                   machine->V[reg1] = rand() & code[1];
                   machine->PC += 2; 
                }
            case 0x0d:
                {

                }

                
        
    }

}

static void push(Chip8State *machine)
{
    machine->SP -= 2;
    machine->memory[machine->SP] = ((machine->PC+2) & 0xFF00) >> 8;
    machine->memory[machine->SP+1] = (machine->PC+2) & 0xFF;
}

static uint16_t pop(Chip8State *machine)
{

}