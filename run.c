#include "cpu.h"
#include "opcodes.h"
#include <stdio.h>

int main() {
    Memory mem;
    CPU cpu;
    reset(&cpu, &mem);
    FILE* fp = fopen("in.hex", "rb");
    fread(mem.data, 1, MAX_MEM, fp);
    fclose(fp);
    {
        u32 temp = 2;
        word startAddress = fetchWord(&cpu, &mem, &temp);
        cpu.programCounter = startAddress;
    }
    execute(&cpu, &mem, 50);
    printf("%x\n", cpu.a);
    return 0;
}
