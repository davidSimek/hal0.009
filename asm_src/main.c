#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
  NUMBER,
  REGISTER,
  INSTRUCTION
} token_type;

typedef enum {
  MOVAR = 0b10000000,
  EXIT  = 0b11000000
} instruction;

char *hasm_source;
size_t hasm_source_size;

FILE *output_file;
char* output_file_name;

void load_hasm(FILE *);
void assemble_hasm();
bool matches_expectation(char* token);
void write_as_bin(token_type type, char *token);
int parsed_int = 0;

token_type expect[4];
int expect_head = 0;

int main(int argc, char** argv) {
  if (argc != 2) {
    perror("You have to provide hasm file.");
    exit(EXIT_FAILURE);
  }

  size_t hasm_file_len = strlen(argv[1]);
  if (hasm_file_len > 100) {
    perror("File name is too long.");
    exit(EXIT_FAILURE);
  }

  if (hasm_file_len < 6) {
    perror("File name is too short.\n");
    exit(EXIT_FAILURE);
  }

  if (argv[1][hasm_file_len - 1] != 'm' ||
      argv[1][hasm_file_len - 2] != 's' ||
      argv[1][hasm_file_len - 3] != 'a' ||
      argv[1][hasm_file_len - 4] != 'h') {
    perror("File has to have \"hasm\" extension.\n");
    exit(EXIT_FAILURE);
  }
  
  FILE* hasm_file = fopen(argv[1], "r");
  if (!hasm_file) {
    perror("Can't open your hasm file. Please check path");
    exit(EXIT_FAILURE);
  }
   
  output_file_name = malloc(hasm_file_len - 1);
  memcpy(output_file_name, argv[1], hasm_file_len - 4);
  output_file_name[hasm_file_len - 1] = 0;
  output_file_name[hasm_file_len - 2 ] = 'n';
  output_file_name[hasm_file_len - 3 ] = 'i';
  output_file_name[hasm_file_len - 4 ] = 'b';
  output_file = fopen(output_file_name, "w");

  if (!output_file) {
    perror("couldn't open output file \n");
    exit(EXIT_FAILURE);
  }

  load_hasm(hasm_file);
  fclose(hasm_file);
  assemble_hasm();

  fclose(output_file);
  printf("Successfully assembled %s", output_file_name);
  exit(EXIT_SUCCESS);
}

void load_hasm(FILE* hasm) {
  fseek(hasm, 0L, SEEK_END);
  hasm_source_size = ftell(hasm);
  fseek(hasm, 0L, SEEK_SET);

  if (hasm_source_size > (100 * 64)) {
    perror("Your source file is too big.\n");
    exit(EXIT_FAILURE);
  }
  hasm_source = (char*)malloc(hasm_source_size + 1);
  if (!hasm_source) {
    perror("Couldn't allocate memory for hasm_souce.\n");
  }
  fread(hasm_source, hasm_source_size, 1, hasm);
}

void assemble_hasm() {
  char* token = strtok(hasm_source, " \n\t");
  expect_head = 0;
  expect[expect_head] = INSTRUCTION;
  while (token) {
    if (strlen(token) == 0) {
      token = strtok(NULL, " \n\t");
      continue;
    }
    if (!matches_expectation(token)) {
      perror("Expected different token here.");
      exit(EXIT_FAILURE);
    }

    write_as_bin(expect[expect_head], token);
    
    token = strtok(NULL, " \n\t");
  }
}

bool matches_expectation(char* token) {
  if (expect[expect_head] == INSTRUCTION) {
    if (strcmp(token, "MOVAR") ||
	strcmp(token, "EXIT")) {
      return true;
    } else {
      printf("Expected instruction, got %s\n", token);
      return false;
    }
  } else if (expect[expect_head] == NUMBER) {
    char* end;
    parsed_int = strtol(token, &end, 10);
    if (*end == '\0' && parsed_int >= 0 && parsed_int < 255) {
      return true;
    } else {
      printf("Expected numebr, got %s\n", token);
      return false;
    }
  } else if (expect[expect_head] == REGISTER) {
    return true;
  } else {
    perror("Expect something, which is not even a choice.");
    exit(EXIT_FAILURE);
  }
  return false;
}

void write_as_bin(token_type type, char *token) {
  if (expect_head < 0 || expect_head > 3) {
    perror("Expect head is out of bounds.\n");
    exit(EXIT_FAILURE);
  }
  
  if (type == INSTRUCTION) {
    if (strcmp(token, "MOVAR") == 0) {
      fputc((uint8_t)MOVAR, output_file);
      expect[0] = INSTRUCTION;
      expect[1] = REGISTER;
      expect[2] = NUMBER;
      expect_head = 2;
    } else if (strcmp(token, "EXIT") == 0) {
      fputc((uint8_t)EXIT, output_file);
      expect[0] = INSTRUCTION;
      expect_head = 0;
    } else {
      perror("Invalid istruction.\n");
      exit(EXIT_FAILURE);
    }
  } else if (type == NUMBER) {
    fputc((uint8_t)parsed_int, output_file);
    --expect_head;
  } else if (type == REGISTER) {
    if (strcmp(token, "RESULT") == 0) {
      putc((uint8_t)0b00000000, output_file);
    } else if (strcmp(token, "A") == 0) {
      putc((uint8_t)0b00000001, output_file);
    } else if (strcmp(token, "B") == 0) {
      putc((uint8_t)0b00000010, output_file);
    } else if (strcmp(token, "C") == 0) {
      putc((uint8_t)0b00000011, output_file);
    } else if (strcmp(token, "D") == 0) {
      putc((uint8_t)0b00000100, output_file);
    } else if (strcmp(token, "E") == 0) {
      putc((uint8_t)0b00000101, output_file);
    } else if (strcmp(token, "F") == 0) {
      putc((uint8_t)0b00000110, output_file);
    } else if (strcmp(token, "G") == 0) {
      putc((uint8_t)0b00000111, output_file);
    } else if (strcmp(token, "H") == 0) {
      putc((uint8_t)0b00001000, output_file);
    } else if (strcmp(token, "I") == 0) {
      putc((uint8_t)0b00001001, output_file);
    } else if (strcmp(token, "J") == 0) {
      putc((uint8_t)0b00001010, output_file);
    } else if (strcmp(token, "K") == 0) {
      putc((uint8_t)0b00001011, output_file);
    } else if (strcmp(token, "L") == 0) {
      putc((uint8_t)0b00001100, output_file);
    } else if (strcmp(token, "M") == 0) {
      putc((uint8_t)0b00001101, output_file);
    } else if (strcmp(token, "N") == 0) {
      putc((uint8_t)0b00001110, output_file);
    } else if (strcmp(token, "O") == 0) {
      putc((uint8_t)0b00001111, output_file);
    } else if (strcmp(token, "P") == 0) {
      putc((uint8_t)0b00010000, output_file);
    } else if (strcmp(token, "SP") == 0) {
      putc((uint8_t)0b00010001, output_file);
    } else if (strcmp(token, "BP") == 0) {
      putc((uint8_t)0b00010010, output_file);
    } else if (strcmp(token, "PC") == 0) {
      putc((uint8_t)0b00010011, output_file);
    } else {
      printf("Invalid register. You used %s\n", token);
    }
    --expect_head;
  } else {
    perror("Invalid type.\n");
  }
}
