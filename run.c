#include "cpu.h"
#include <stdio.h>
#include "opcodes.h"

int main() {
    Memory mem;
    CPU cpu;
    reset(&cpu, &mem);
    //
    mem.data[0xfffc] = LDA_ZP;
    mem.data[0xfffd] = 0x69;
    mem.data[0x0069] = 0x41;
    // mem.data[0x5657] = 0x69;
    //
    execute(&cpu, &mem, 3);
    printf("%x\n", cpu.a);
    return 0;
}
