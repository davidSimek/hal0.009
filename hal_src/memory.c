// WTBP -> word (2 bytes) to binary pattern
// WTB  -> word (2 bytes) to binary
#define WTBP "%c%c%c%c%c%c%c%c %c%c%c%c%c%c%c%c"

#define WTB(word)  \
  ((word) & 0x8000 ? '1' : '0'), \
  ((word) & 0x4000 ? '1' : '0'), \
  ((word) & 0x2000 ? '1' : '0'), \
  ((word) & 0x1000 ? '1' : '0'), \
  ((word) & 0x0800 ? '1' : '0'), \
  ((word) & 0x0400 ? '1' : '0'), \
  ((word) & 0x0200 ? '1' : '0'), \
  ((word) & 0x0100 ? '1' : '0'), \
  ((word) & 0x0080 ? '1' : '0'), \
  ((word) & 0x0040 ? '1' : '0'), \
  ((word) & 0x0020 ? '1' : '0'), \
  ((word) & 0x0010 ? '1' : '0'), \
  ((word) & 0x0008 ? '1' : '0'), \
  ((word) & 0x0004 ? '1' : '0'), \
  ((word) & 0x0002 ? '1' : '0'), \
  ((word) & 0x0001 ? '1' : '0')

#include "memory.h"
#include "error.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>

uint8_t *program_memory = 0;
size_t program_memory_size = 0;
uint8_t *user_memory = 0;

uint16_t registers[20] = {0};

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
  printf("Result: "WTBP"\n", WTB(registers[RESULT]));
  printf("     a: "WTBP"\n", WTB(registers[A]));
  printf("     b: "WTBP"\n", WTB(registers[B]));
  printf("     c: "WTBP"\n", WTB(registers[C]));
  printf("     d: "WTBP"\n", WTB(registers[D]));
  printf("     e: "WTBP"\n", WTB(registers[E]));
  printf("     f: "WTBP"\n", WTB(registers[F]));
  printf("     g: "WTBP"\n", WTB(registers[G]));
  printf("     h: "WTBP"\n", WTB(registers[H]));
  printf("     i: "WTBP"\n", WTB(registers[I]));
  printf("     j: "WTBP"\n", WTB(registers[J]));
  printf("     k: "WTBP"\n", WTB(registers[K]));
  printf("     l: "WTBP"\n", WTB(registers[L]));
  printf("     m: "WTBP"\n", WTB(registers[M]));
  printf("     n: "WTBP"\n", WTB(registers[N]));
  printf("     o: "WTBP"\n", WTB(registers[O]));
  printf("     p: "WTBP"\n", WTB(registers[P]));
  printf("    sp: "WTBP"\n", WTB(registers[SP]));
  printf("    bp: "WTBP"\n", WTB(registers[BP]));
  printf("    pc: "WTBP"\n", WTB(registers[PC]));
}
