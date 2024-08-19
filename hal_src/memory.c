// BTBP -> byte to binary pattern
// BTB -> byte to binary
#define BTBP "%c%c%c%c%c%c%c%c"
#define BTB(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 


#include "memory.h"
#include "error.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>

uint8_t *program_memory = 0;
size_t program_memory_size = 0;
uint8_t *user_memory = 0;

uint8_t registers[20] = {0};

char* program_name;

void init_program_memory(int argc, char **argv) {
  if (argc != 2) {
    hal_exit(WRONG_ARGUMENT_COUNT);
  }
  
  FILE* bytecode_file = fopen(argv[1], "r");
  if (!bytecode_file) {
    hal_exit(CANT_OPEN_FILE);
  }

  program_name = argv[1];
  
  fseek(bytecode_file, 0L, SEEK_END);
  size_t bytecode_file_size = ftell(bytecode_file);
  program_memory_size = bytecode_file_size + 1;
  fseek(bytecode_file, 0L, SEEK_SET);

  if (bytecode_file_size > MAX_PROGRAM_MEMORY_SIZE) {
    fclose(bytecode_file);
    hal_exit(FILE_TOO_BIG);
  }

  program_memory = (uint8_t*)malloc(program_memory_size);
  if (!program_memory) {
    hal_exit(MALLOC_ERROR);
  }

  fread(program_memory, bytecode_file_size + 1, 1, bytecode_file);
  program_memory[program_memory_size] = 0;
  
  fclose(bytecode_file);
}

void clean_program_memory() {
  free(program_memory);
}

void init_user_memory() {
  user_memory = (uint8_t*)malloc(USER_MEMORY_SIZE);
  if (!user_memory) {
    hal_exit(MALLOC_ERROR);
  }
  memset(user_memory, 0, USER_MEMORY_SIZE);
}
void clean_user_memory() {
  free(user_memory);
}

void dump_registers() {
  printf("Registers dump.\n");
  printf("Result: "BTBP"\n", BTB(registers[RESULT]));
  printf("     a: "BTBP"\n", BTB(registers[A]));
  printf("     b: "BTBP"\n", BTB(registers[B]));
  printf("     c: "BTBP"\n", BTB(registers[C]));
  printf("     d: "BTBP"\n", BTB(registers[D]));
  printf("     e: "BTBP"\n", BTB(registers[E]));
  printf("     f: "BTBP"\n", BTB(registers[F]));
  printf("     g: "BTBP"\n", BTB(registers[G]));
  printf("     h: "BTBP"\n", BTB(registers[H]));
  printf("     i: "BTBP"\n", BTB(registers[I]));
  printf("     j: "BTBP"\n", BTB(registers[J]));
  printf("     k: "BTBP"\n", BTB(registers[K]));
  printf("     l: "BTBP"\n", BTB(registers[L]));
  printf("     m: "BTBP"\n", BTB(registers[M]));
  printf("     n: "BTBP"\n", BTB(registers[N]));
  printf("     o: "BTBP"\n", BTB(registers[O]));
  printf("     p: "BTBP"\n", BTB(registers[P]));
  printf("    sp: "BTBP"\n", BTB(registers[SP]));
  printf("    bp: "BTBP"\n", BTB(registers[BP]));
  printf("    pc: "BTBP"\n", BTB(registers[PC]));
}
