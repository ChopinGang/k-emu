#include "cpu.h"
#include "opcodes.h"
#include <stdio.h>

int main() {
    Memory mem;
    CPU cpu;
    reset(&cpu, &mem);
    // starting stuff
    mem.data[0xfffc] = 0x42;
    mem.data[0xfffd] = 0x69;
    
    //
    execute(&cpu, &mem, 50);
    printf("%x\n", cpu.a);
    return 0;
}
