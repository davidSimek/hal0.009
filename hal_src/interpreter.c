#include "interpreter.h"
#include "error.h"
#include "memory.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum {
  MOV = 0
} memory_instructions;

typedef enum {
  EXIT = 0
} flow_instructions;

typedef enum {
  INVALID = 0,
     MATH = 1,
   MEMORY = 2,
     FLOW = 3
} general_type;

static bool should_run = true;

void handle_instruction();

// executes instruction and return amoung of arguments it expects
int execute_instruction();

int execute_math_instruction(uint8_t instruction);
int execute_memory_instruction(uint8_t instruction);
int execute_flow_instruction(uint8_t instruction);

void execute_mov(uint8_t instruction);
void execute_exit(uint8_t instruction);

uint8_t arg1();
uint8_t arg2();

void run() {
  while (should_run) {
    if (registers[PC] > USER_MEMORY_SIZE - 1) {
      hal_exit(DIDNT_EXIT_PROGRAM);
    }
    handle_instruction();
  }
}

void handle_instruction() {
  int move_by = execute_instruction();
  registers[PC] += move_by;
}

int execute_instruction() {
  int move_by = 1;
  uint8_t instruction = program_memory[registers[PC]];
  uint8_t general_type = instruction >> 6;
  switch (general_type) {
  case INVALID:
    printf("Instruction %d has invalid type.\n", instruction);
    exit(EXIT_FAILURE);
    break;
  case MATH:
    return execute_math_instruction(instruction);
    break;
  case MEMORY:
    return execute_memory_instruction(instruction);
    break;
  case FLOW:
    return execute_flow_instruction(instruction);
    break;
  }
}

int execute_math_instruction(uint8_t instruction) {
  return 1;
}

int execute_flow_instruction(uint8_t instruction) {
  uint8_t instruction_type = instruction & 0b00001111;
  switch (instruction_type) {
  case EXIT:
    should_run = false;
    return 1;
    break;
  default:
    printf("invalid instruction %d in execute_memory_instruction()\n");
    exit(EXIT_FAILURE);
  }
}
int execute_memory_instruction(uint8_t instruction) {
  uint8_t instruction_type = instruction & 0b00001111;
  switch (instruction_type) {
  case MOV:
    execute_mov(instruction);
    return 3;
    break;
  default:
    printf("Invalid instruction %d in executer_flow_instruction()\n", instruction);
    exit(EXIT_FAILURE);
  }
}

void execute_mov(uint8_t instruction) {
  uint8_t variant = (instruction & 0b00110000) >> 4;
  switch (variant) {
  case 0:
    registers[arg2()] = arg1();
    break;
  case 1:
    registers[arg2()] = registers[arg1()];
    break;
  case 2:
    user_memory[arg2()] = registers[arg1()];
    break;
  case 3:
    registers[arg2()] = user_memory[arg1()];
    break;
  }
}

uint8_t arg1() {
  return program_memory[registers[PC] + 1];
}

uint8_t arg2() {
  return program_memory[registers[PC] + 2];
}
