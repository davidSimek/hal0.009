#include "interpreter.h"
#include "error.h"
#include "memory.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum {
  INC = 0,
  DEC = 1,
  CMP = 2
} math_instructions;

typedef enum {
  MOV = 0
} memory_instructions;

typedef enum { EXIT = 0, JMP = 1 } flow_instructions;

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

void execute_inc(uint8_t instruction);
void execute_dec(uint8_t instruction);
void execute_cmp(uint8_t instruction);

void execute_mov(uint8_t instruction);

void execute_exit(uint8_t instruction);
void execute_jmp(uint8_t instruction);

uint8_t arg1();
uint8_t arg2();

void run() {
  while (should_run) {
    if (registers[PC] > USER_MEMORY_SIZE - 2) {
      hal_exit(DIDNT_EXIT_PROGRAM);
    }
    handle_instruction();
  }
}

void handle_instruction() {
  int move_by = execute_instruction();
  printf("moving by %d\n", move_by);
  registers[PC] += move_by;
}

int execute_instruction() {
  uint8_t instruction = program_memory[registers[PC]];
  uint8_t general_type = instruction >> 6;
  switch (general_type) {
  case INVALID:
    printf("Instruction %d on address %d has invalid type.\n", instruction, registers[PC]);
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
  printf("Instruction %d on address %d has invalid type.\n", instruction, registers[PC]);
  exit(EXIT_FAILURE);
}

// executes math instruction, and returns how many uint8_ts till next instruction including instruction itself
int execute_math_instruction(uint8_t instruction) {
  uint8_t instruction_type = instruction & 0b00001111;
  switch (instruction_type) {
  case INC:
    execute_inc(instruction);
    return 2;
  case DEC:
    execute_dec(instruction);
    return 2;
  case CMP:
    execute_cmp(instruction);
    return 3;
  default:
      printf("invalid instruction %d in execute_math_instruction()\n", instruction);
      exit(EXIT_FAILURE);
  }
}

// executes flow instruction, and returns how many uint8_ts till next instruction including instruction itself
int execute_flow_instruction(uint8_t instruction) {
  uint8_t instruction_type = instruction & 0b00001111;
  switch (instruction_type) {
  case EXIT:
    should_run = false;
    return 1;
    break;
  case JMP:
    execute_jmp(instruction);
    return 0;
    break;
  default:
    printf("invalid instruction %d in execute_memory_instruction()\n", instruction);
    exit(EXIT_FAILURE);
  }
}

// executes memory instruction, and returns how many uint8_ts till next instruction including instruction itself
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

void execute_jmp(uint8_t instruction) {
  (void)(instruction);
  registers[PC] = arg1();
}

void execute_inc(uint8_t instruction) {
  (void)(instruction);
  ++registers[arg1()];
}
void execute_dec(uint8_t instruction) {
  (void)(instruction);
  --registers[arg1()];
}

void execute_cmp(uint8_t instruction) {
  (void)(instruction);
  registers[RESULT] = (registers[arg1()] == registers[arg2()]) ? 1 : 0;
}

uint8_t arg1() {
  return program_memory[registers[PC] + 1];
}

uint8_t arg2() {
  return program_memory[registers[PC] + 2];
}

void kill_hal() {
  should_run = false;
}
