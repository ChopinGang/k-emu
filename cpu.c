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

byte fetchByte(CPU* cpu, Memory* mem, u32* cycles) {
    byte data = mem->data[cpu->programCounter];
    cpu->programCounter++;
    *cycles -= 1;
    return data;
}

word fetchWord(CPU* cpu, Memory* mem, u32* cycles) {
    word data = mem->data[cpu->programCounter] << 8;
    cpu->programCounter++;
    data |= (mem->data[cpu->programCounter]);
    cpu->programCounter++;
    *cycles -= 2;
    return data;
}

byte readByte(CPU* cpu, Memory* mem, u32* cycles, word addr) {
    byte data = mem->data[addr];
    *cycles -= 1;
    return data;
}

word readWord(CPU* cpu, Memory* mem, u32* cycles, word addr) {
    word data = mem->data[addr] << 8;
    data |= mem->data[addr + 1];
    *cycles -= 1;
    return data;
}

void loadSetStatus(CPU* cpu) {
    cpu->z = (cpu->a == 0);
    cpu->n = (cpu->a & 0b10000000) > 0;
}

void execute(CPU* cpu, Memory* mem, u32 cycles) {
    while (cycles > 0) {
        byte instr = fetchByte(cpu, mem, &cycles);
        switch (instr) {
            case LDA_IM: {
                byte value = fetchByte(cpu, mem, &cycles);
                cpu->a = value;
                loadSetStatus(cpu);
            }   break;
            case LDA_AB: {
                word addr = fetchWord(cpu, mem, &cycles); // goo goo ga ga
                cpu->a = mem->data[addr];
                loadSetStatus(cpu);
            }   break;
            case LDA_ZP: {
                byte zeroPageAddr = fetchByte(cpu, mem, &cycles);
                cpu->a = readByte(cpu, mem, &cycles, zeroPageAddr);
                loadSetStatus(cpu);
            }   break;
            case JMP_AB: {
                word addr = fetchWord(cpu, mem, &cycles);
                cpu->programCounter = addr;
            }   break;
            case JMP_IN: {
                word addr = fetchWord(cpu, mem, &cycles);
                word value = readWord(cpu, mem, &cycles, addr);
                cpu->programCounter = value;
            }   break;
            default: {
                printf("Instruction not handled: %d : %d\n", instr, cycles);
            }   break;
        }
    }
}
