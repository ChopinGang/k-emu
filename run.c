#include "cpu.h"

int main() {
    Memory mem;
    CPU cpu;
    reset(&cpu, &mem);
    execute(&cpu, &mem, 2);
    return 0;
}
