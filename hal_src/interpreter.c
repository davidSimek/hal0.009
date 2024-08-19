#include "interpreter.h"
#include "error.h"
#include "memory.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  MOVAR = 0b10000000,
  MOVRR = 0b10010000,
  MOVRM = 0b10100000,
  MOVMR = 0b10110000,
  EXIT  = 0b11000000
} instruction;

static bool should_run = true;

void handle_instruction();

// executes instruction and return amoung of arguments it expects
int execute_instruction();

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
  printf("executing instruction %d\n", instruction);
  switch (instruction) {
  case MOVAR:
    uint8_t argument = program_memory[registers[PC + 1]];
    uint8_t reg = program_memory[registers[PC + 2]];
    registers[reg] = argument;
    printf("instruction: MOVAR\n");
    move_by = 3;
    break;
  case EXIT:
    printf("instruction: EXIT\n");
    move_by = 0;
    hal_exit(SUCCESS);
    break;
  default:
    hal_exit(ILLEGAL_INSTRUCTION);
    break;
  }
  return move_by;
}
