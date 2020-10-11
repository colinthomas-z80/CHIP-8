#include "header.h"

int main(int argc, char* argv[])
{
    FILE *f = fopen(argv[1], "rb");
    if(f == NULL){
        printf("Could not open file");
        exit(1);
    }

    //get file size
    fseek(f, 0L, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    // chip 8 puts programs in memory at 0x200
    // the hardcoded addresses in programs will expect this

    //read file into memory at 0x200 and close it
    unsigned char *buffer = malloc(fsize+0x200);
    fread(buffer+0x200, fsize, 1, f);
    fclose(f);

    int pc = 0x200;
    while(pc < (fsize + 0x200)){
        disassemble(buffer,pc);
        pc += 2;
        printf("\n");
    }
}