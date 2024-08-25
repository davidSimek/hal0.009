#ifndef MEMORY_H
#define MEMORY_H

typedef enum {
  RESULT = 0b00000000,
       A = 0b00000001,
       B = 0b00000010,
       C = 0b00000011,
       D = 0b00000100,
       E = 0b00000101,
       F = 0b00000110,
       G = 0b00000111,
       H = 0b00001000,
       I = 0b00001001,
       J = 0b00001010,
       K = 0b00001011,
       L = 0b00001100,
       M = 0b00001101,
       N = 0b00001110,
       O = 0b00001111,
       P = 0b00010000,
      SP = 0b00010001,
      BP = 0b00010010,
      PC = 0b00010011
} reg;
  
#include <stdint.h>
#include <stdlib.h>

#define MAX_PROGRAM_MEMORY_SIZE 65536
#define USER_MEMORY_SIZE        65536

extern uint8_t *program_memory;
extern size_t program_memory_size;
extern uint8_t *user_memory;

extern uint16_t registers[20];

extern char* program_name;

void init_program_memory(int argc, char** argv);
void clean_program_memory();
void init_user_memory();
void clean_user_memory();
void dump_registers();
#endif
