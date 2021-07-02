#include "cpu.h"
#include "opcodes.h"

#include <stdio.h>
#include <stdlib.h>

void initialize(Memory* mem) {
    for (u32 i = 0; i < MAX_MEM; i++) {
        mem->data[i] = 0;
    }
}

void reset(CPU* cpu, Memory* mem) {
    cpu->programCounter = 0xfffc;
    cpu->stackPointer = 0x0100;
    cpu->c = cpu->z = cpu->i = cpu->d = cpu->b = cpu->o = cpu->n = 0;
    cpu->a = cpu->x = cpu->y = 0;
    initialize(mem);
}

byte fetch(CPU* cpu, Memory* mem, u32* cycles) {
    byte data = mem->data[cpu->programCounter];
    cpu->programCounter++;
    cycles--;
    return data;
}

void execute(CPU* cpu, Memory* mem, u32 cycles) {
    while (cycles > 0) {
        byte instr = fetch(cpu, mem, &cycles);
        switch (instr) {
            case LDA_IM: {
                byte value = fetch(cpu, mem, &cycles);
                cpu->a = value;
                break;
            }
            case LDA_AB: {
                byte value = mem->data[fetch(cpu, mem, &cycles)];
                cpu->a = value;
            }
            default:
                break;
        }
    }
}
