#pragma once

#define byte unsigned char
#define word unsigned short
#define u32 unsigned int
#define MAX_MEM 1024 * 64

typedef struct Memory {
    byte data[MAX_MEM];
} Memory;

typedef struct CPU {

    word programCounter;
    byte stackPointer;

    // registers
    byte a, x, y;

    // flags
    byte c : 1; // carry
    byte z : 1; // zero
    byte i : 1; // interrupt disable
    byte d : 1; // decimal
    byte b : 1; // break
    byte v : 1; // overflow
    byte n : 1; // negative

} CPU;

void reset(CPU* cpu, Memory* mem);
void initialize(Memory* mem);
void execute(CPU* cpu, Memory* mem, u32 cycles);
word readWord(CPU* cpu, Memory* mem, u32* cycles, word addr);
byte readByte(CPU* cpu, Memory* mem, u32* cycles, word addr);
word fetchWord(CPU* cpu, Memory* mem, u32* cycles);
byte fetchByte(CPU* cpu, Memory* mem, u32* cycles);
void loadSetStatus(CPU* cpu);
void pushWord(CPU* cpu, Memory* mem, word data, u32* cycles);
void pushByte(CPU* cpu, Memory* mem, byte data, u32* cycles);
byte popByte(CPU* cpu, Memory* mem, u32* cycles);
word popWord(CPU* cpu, Memory* mem, u32* cycles);
void addSetFlags(CPU* cpu, word result);
