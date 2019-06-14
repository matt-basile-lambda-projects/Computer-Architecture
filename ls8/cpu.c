#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6
#define SP 7

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *filename)
{
   int address = 0;
    FILE *fp;
    char line[1024];
    fp =fopen(filename, "r"); //read the data
    if(fp ==NULL){
      fprintf(stderr, "file not found\n");
      exit(1);
    }
    // do stuff -> take the contents and load in memory
   while(fgets(line, sizeof line, fp) != NULL){
      char *endptr;
      unsigned char v = strtoul(line, &endptr, 2); //str to unsigned long
      // printf("%c", v);
      if (endptr == line){
        continue;
      }
      cpu->ram[address++] = v;
   };
   fclose(fp); // close the file
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  unsigned char *reg = cpu->registers;
  switch (op) {
    case ALU_MUL:
      // TODO
      // multiple registerA and registerB
      reg[regA] *= reg[regB];
      // printf("%c", regA);
      break;
    case ALU_ADD:
      reg[regA] += reg[regB];
      break;
    case ALU_CMP:
      if(reg[regA] == reg[regB]){
         cpu->fl =  0b00000001;
      }
      else if(reg[regA] < reg[regB]){
         cpu->fl =  0b00000100;
      }else{
        cpu->fl =  0b00000010;
      }
      break;
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
    unsigned char *ram = cpu->ram;
    unsigned char IR = ram[PC];
    // 2. Figure out how many operands this next instruction requires
    int num_operands = (IR >> 6);
    int instruction_bytes = num_operands +1;
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operandA = cpu_ram_read(cpu, PC+1);
    unsigned char operandB = cpu_ram_read(cpu, PC+2);
    // 4. switch() over it to decide on a course of action.
    switch (IR){
      // 5. Do whatever the instruction should do according to the spec.
      // DAY 1 HLT, LDI, PRN
      case LDI:
        cpu->registers[operandA] = operandB;
        break;
      case PRN:
        printf("%d\n", cpu->registers[operandA]);
        break;
      case MUL:
          alu(cpu, ALU_MUL, operandA, operandB);
          break;
      case ADD:
          // printf("%s\n", 'adding');
          alu(cpu, ALU_ADD, operandA, operandB);
          break;
      case PUSH:
        cpu->registers[SP]--; // decrement SP
				unsigned char v = cpu->registers[operandA]; // now push v on stack
				cpu->ram[cpu->registers[SP]] = v; 
         break;
      case POP:
				v = cpu->ram[cpu->registers[SP]];
				cpu->registers[operandA] = v;
				cpu->registers[SP]++;
        break;
      case CALL:
				cpu->registers[SP]--;
        cpu->ram[cpu->registers[SP]] = cpu->pc + 2;
        cpu->pc = cpu->registers[operandA];
        instruction_bytes = 0;
        break;
      case RET:
        cpu->pc = cpu->ram[cpu->registers[SP]];
        cpu->registers[SP]++;
        instruction_bytes = 0;
        break;
      case JMP:
        cpu->pc = cpu->ram[cpu->registers[operandA]];
        instruction_bytes =0;
        break;
      case CMP:
        alu(cpu, ALU_CMP, operandA, operandB);
        break;
      case JEQ:
        if(cpu->fl == 1){
          cpu->pc = cpu->registers[operandA];
          instruction_bytes =0;
        } 
        break;
      case JNE:
         if(cpu->fl > 1){
            cpu->pc = cpu->registers[operandA];
            instruction_bytes = 0;
          } 
        break;
      case HLT:
        running = 0;
        break;
    default:
        printf("Unknown instruction %02x at address %02x\n");
        exit(3);
    }
    // 6. Move the PC to the next instruction.
    cpu->pc += instruction_bytes;
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
  // Initialize flag to 0
  cpu->fl = 0;
  // Initialize 8 bit space for registers to 0
  memset(cpu->registers, 0, 8);
  // Initialize Registers SP
  cpu->registers[SP] = 0xF4;
  // Initialize 256 bytes for RAM to 0
  memset(cpu->ram, 0, 256);
}
