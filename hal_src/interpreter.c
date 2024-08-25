#include "interpreter.h"
#include "error.h"
#include "memory.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

int execute_mov(uint8_t instruction);

void execute_exit(uint8_t instruction);
void execute_jmp(uint8_t instruction);

int last_arg_size = 0;

uint8_t arg_1_8();   // size 1B offset  8b
uint8_t arg_1_16();  // size 1B offset 16b
uint8_t arg_1_24();  // size 1B offset 24b
uint16_t arg_2_8();  // size 2B offset  8b
uint16_t arg_2_16(); // size 2B offset 16b
uint16_t arg_2_24(); // size 2B offset 24b


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
  registers[PC] += move_by;
}

int execute_instruction() {
  uint8_t instruction = program_memory[registers[PC]];
  uint8_t general_type = instruction >> 6;
  last_arg_size = 0;
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
    return execute_mov(instruction);
  default:
    printf("Invalid instruction %d in executer_flow_instruction()\n", instruction);
    exit(EXIT_FAILURE);
  }
}

int execute_mov(uint8_t instruction) {
  uint8_t variant = (instruction & 0b00110000) >> 4;
  switch (variant) {
  case 0:
    registers[arg_1_24()] = arg_2_8();
    return 4;
  case 1:
    registers[arg_1_16()] = registers[arg_1_8()];
    return 3;
  case 2:
    user_memory[arg_2_16()] = registers[arg_1_8()];
    return 4;
  default:
    registers[arg_1_24()] = user_memory[arg_2_8()];
    return 4;
  }
}

void execute_jmp(uint8_t instruction) {
  (void)(instruction);
  registers[PC] = arg_2_8();
}

void execute_inc(uint8_t instruction) {
  (void)(instruction);
  ++registers[arg_2_8()];
}
void execute_dec(uint8_t instruction) {
  (void)(instruction);
  --registers[arg_2_8()];
}

void execute_cmp(uint8_t instruction) {
  (void)(instruction);
  registers[RESULT] = (registers[arg_1_8()] == registers[arg_1_16()]) ? 1 : 0;
}

uint8_t arg_1_8() {
  return program_memory[registers[PC] + 1];
}

uint8_t arg_1_16() {
  return program_memory[registers[PC] + 2];
}

uint8_t arg_1_24(){
  return program_memory[registers[PC] + 3];
}

uint16_t arg_2_8() {
  uint16_t result = program_memory[registers[PC] + 1] << 8; // 1. byte
  result += program_memory[registers[PC] + 2];              // 2. byte
  return result;
}

uint16_t arg_2_16() {
  uint16_t result = program_memory[registers[PC] + 2] << 8; // 1. byte
  result += program_memory[registers[PC] + 3];              // 2. byte
  return result;
}

uint16_t arg_2_24() {
  uint16_t result = program_memory[registers[PC] + 3] << 8; // 1. byte
  result += program_memory[registers[PC] + 4];              // 2. byte
  return result;
}

void kill_hal() {
  should_run = false;
}
