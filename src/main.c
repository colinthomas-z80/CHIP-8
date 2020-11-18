#include <Windows.h>
#include "header.h"

void delay(int sec);

HANDLE writeHandle;
HANDLE readHandle;



int main(int argc, char* argv[])
{
    /* SETUP WINDOW */
    writeHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    readHandle = GetStdHandle(STD_INPUT_HANDLE);

    SetConsoleTitle(TEXT("CHIP-8 EMULATOR"));

    SMALL_RECT windowSize = {0, 0, 79, 49};
    SetConsoleWindowInfo(writeHandle, TRUE, &windowSize);

    COORD bufferSize = {80, 50};
    SetConsoleScreenBufferSize(writeHandle, bufferSize);

    COORD characterPos = {0, 0};

    CHAR_INFO consoleBuffer[80*50];

    for(int y = 0; y < 50; ++y){
        for(int x = 0; x < 80; ++x){
            consoleBuffer[x + 80*y].Char.AsciiChar = rand() % 256;

            consoleBuffer[x + 80*y].Attributes = rand() % 256;
        }
    }

    WriteConsoleOutputA(writeHandle, consoleBuffer, bufferSize, characterPos, &windowSize);

    // /* LOAD FILE */
    // FILE *f = fopen(argv[1], "rb");
    // if(f == NULL){
    //     printf("Could not open file");
    //     exit(1);
    // }
    // fseek(f, 0L, SEEK_END);
    // int fsize = ftell(f);
    // fseek(f, 0L, SEEK_SET);

    // /* MACHINE SETUP */
    // Chip8State *machine = InitChip8();
    // fread(machine->memory+0x200, fsize, 1, f);

    // /* RUN */
    // for(int i = 0x200; i < fsize + 0x200; i++)
    // {
    //     emulate(machine);
    //     //delay(1);
    //     printf("\n");
    // }
    
    printf("Terminated Successfully");
}

void delay(int sec)
{
    for(int i = 0; i < sec*10000000; i++)
        ;
}


    // //read file into memory at 0x200 and close it
    // unsigned char *buffer = malloc(fsize+0x200);
    // fread(buffer+0x200, fsize, 1, f);
    // fclose(f);

    // int pc = 0x200;
    // while(pc < (fsize + 0x200)){
    //     disassemble(buffer,pc);
    //     pc += 2;
    //     printf("\n");
    // }