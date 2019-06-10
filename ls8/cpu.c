#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6
#define HLT 1
#define LDI 2
#define PRN 3

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
    // From print8.ls8
    0b10000010, // LDI R0,8
    0b00000000,
    0b00001000,
    0b01000111, // PRN R0
    0b00000000,
    0b00000001  // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++) {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op) {
    case ALU_MUL:
      // TODO
      break;

    // TODO: implement more ALU ops
  }
}

/* RAM Functions */
// Read
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char index ){
  // return values in our RAM
  return cpu->ram[index];
}
// Write
void cpu_ram_write(struct cpu *cpu, unsigned char index, unsigned char item){
  // write in the given index
  cpu->ram[index] = item;
}
/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    int PC = cpu->pc;
    unsigned char ir = cpu_ram_read(cpu, PC);
    // 2. Figure out how many operands this next instruction requires
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, PC+1);
    unsigned char operandB = cpu_ram_read(cpu, PC+2);
    // 4. switch() over it to decide on a course of action.
    switch (ir){
      // 5. Do whatever the instruction should do according to the spec.
      // DAY 1 HLT, LDI, PRN
      case LDI:
        cpu->registers[operandA] = operandB;
        PC+=2;
        break;
      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        break;
      case HLT:
        running = 0;
        break;
    default:
        printf("Unknown instruction %02x at address %02x\n");
        exit(1);
    }
    // 6. Move the PC to the next instruction.
    PC++;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // Initialize program counter to 0 
  cpu->pc = 0;
  // Initialize instruction register to 0
  cpu->ir = 0;
  // Initalize 8 bit space for registers to 0
  memset(cpu->registers, 0, 7);
  // Initialize 256 bytes for RAM to 0
  memset(cpu->ram, 0, 256);
}
