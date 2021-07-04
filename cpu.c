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
    cpu->stackPointer = 255;
    cpu->c = cpu->z = cpu->i = cpu->d = cpu->b = cpu->v = cpu->n = 0;
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
    *cycles -= 2;
    return data;
}

void writeByte(CPU* cpu, Memory* mem, u32* cycles, byte data, word addr) {
    mem->data[addr] = data;
    *cycles -= 1;
}

void writeWord(CPU* cpu, Memory* mem, u32* cycles, word data, word addr) {
    mem->data[addr] = (byte) (data << 8);
    mem->data[addr + 1] = (byte) (data & 0xff);
    *cycles -= 2;
}

void loadSetStatus(CPU* cpu) {
    cpu->z = (cpu->a == 0);
    cpu->n = (cpu->a & 0b10000000) > 0;
}

void pushWord(CPU* cpu, Memory* mem, word data, u32* cycles) {
    cpu->stackPointer++;
    mem->data[0x0100 + cpu->stackPointer] = (byte)(data >> 8);
    cpu->stackPointer++;
    mem->data[0x0100 + cpu->stackPointer] = (byte)(data & 0xff);
    *cycles -= 2;
}

void pushByte(CPU* cpu, Memory* mem, byte data, u32* cycles) {
    cpu->stackPointer++;
    mem->data[0x0100 + cpu->stackPointer] = data;
    *cycles -= 1;
}

byte popByte(CPU* cpu, Memory* mem, u32* cycles) {
    byte value = mem->data[cpu->stackPointer + 0x0100];
    mem->data[cpu->stackPointer + 0x0100] = 0x00;
    cpu->stackPointer--;
    *cycles -= 1;
    return value;
}

word popWord(CPU* cpu, Memory* mem, u32* cycles) {
    word value = mem->data[cpu->stackPointer + 0x0100];
    cpu->stackPointer--;
    value |= mem->data[cpu->stackPointer + 0x0100] << 8;
    cpu->stackPointer--;
    *cycles -= 2;
    return value;
}

void addSetFlags(CPU* cpu, word result) {
    cpu->c = result > 255;
    cpu->z = result == 0;
    cpu->n = (result & 0x80) > 0;
    cpu->v = (result & 0x100) > 0;
}

void execute(CPU* cpu, Memory* mem, u32 cycles) {
    u32 starting;
    while (cycles > 0) {
        starting = cpu->programCounter;
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
                cycles--;
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
            case JSR_AB: {
                word addr = fetchWord(cpu, mem, &cycles);
                pushWord(cpu, mem, (word) (starting), &cycles);
                cpu->programCounter = addr;
                cycles--;
            }   break;
            case RTS_IM: {
                word addr = popWord(cpu, mem, &cycles);
                cpu->programCounter = addr;
                cpu->programCounter++;
                cycles -= 3;
            }   break;
            case NOP_IM: {
                cycles--;
            }   break;
            case STP_IM: {
                cycles = 0;
            }   break;
            case ADC_IM: {
                byte value = fetchByte(cpu, mem, &cycles);
                word result = value + cpu->a + cpu->c;
                addSetFlags(cpu, result);
                cpu->a = (byte) result;
            }   break;
            case ADC_AB: {
                word addr = fetchWord(cpu, mem, &cycles);
                byte value = readByte(cpu, mem, &cycles, addr);
                word result = value + cpu->a + cpu->c;
                addSetFlags(cpu, result);
                cpu->a = (byte) result;
            }   break;
            case SEC_IM: {
                cpu->c = 0x1;
                cycles--;
            }   break;
            case CLC_IM: {
                cpu->c = 0x0;
                cycles--;
            }   break;
            case CLV_IM: {
                cpu->v = 0x0;
                cycles--;
            }   break;
            default: {
                printf("Instruction not handled: %x : %x\n", instr, cpu->programCounter);
            }   break;
        }
        printf("%x: %x | %x - %d\n", starting, instr, cpu->a, cycles);
    }
}
