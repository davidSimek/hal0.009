#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define INSTRUCTION_MAP_SIZE 18

typedef enum {
  NUMBER      = 0,
  REGISTER    = 1,
  INSTRUCTION = 2,
  LABEL       = 3,
  NONE        = 4
} token_type;

typedef enum {
  // math instructions
  INC   = 0b01000000,
  DEC   = 0b01000001,
  CMP   = 0b01000010,
  
  // memory instructions
  MOVAR = 0b10000000,
  MOVRR = 0b10010000,
  MOVRM = 0b10100000,
  MOVMR = 0b10110000,

  // flow instructions
  EXIT  = 0b11000000,
  JMP   = 0b11000001
} hal_instruction;

#define REGISTER_COUNT 20

typedef enum {
  RESULT = 0,
  A      = 1,
  B      = 2,
  C      = 3,
  D      = 4,
  E      = 5,
  F      = 6,
  G      = 7,
  H      = 8,
  I      = 9,
  J      = 10,
  K      = 11,
  L      = 12,
  M      = 13,
  N      = 14,
  O      = 15,
  P      = 16,
  SP     = 17,
  BP     = 18,
  PC     = 19
} hal_register;

typedef struct {
  char* token;
  hal_instruction instruction;
  int expect;
  token_type arg1;
  token_type arg2;
} instruction_entry;

typedef struct {
  char* token;
  hal_register hal_register;
} register_entry;

typedef struct {
  char* label;
  uint8_t address;
} label_entry;

instruction_entry instruction_map[INSTRUCTION_MAP_SIZE] = {
  // math instruction
  {"INC",   INC,   1, REGISTER, NONE},
  {"inc",   INC,   1, REGISTER, NONE},
  {"DEC",   DEC,   1, REGISTER, NONE},
  {"dec",   DEC,   1, REGISTER, NONE},
  {"CMP",   CMP,   2, REGISTER, REGISTER},
  {"cmp",   CMP,   2, REGISTER, REGISTER},
  
  
  // memory instructions
  {"MOVAR", MOVAR, 2, NUMBER,   REGISTER},
  {"movar", MOVAR, 2, NUMBER,   REGISTER},
  {"MOVRR", MOVRR, 2, REGISTER, REGISTER},
  {"movrr", MOVRR, 2, REGISTER, REGISTER},
  {"MOVRM", MOVRM, 2, REGISTER, NUMBER},
  {"movrm", MOVRM, 2, REGISTER, NUMBER},
  {"MOVMR", MOVMR, 2, NUMBER,   REGISTER},
  {"movmr", MOVMR, 2, NUMBER,   REGISTER},

  // flow instructions
  {"EXIT",  EXIT,  0, NONE,     NONE},
  {"exit",  EXIT,  0, NONE,     NONE},
  {"JMP",   JMP,   1, LABEL,    NONE},
  {"jmp",   JMP,   1, LABEL,    NONE}
};

register_entry register_map[REGISTER_COUNT] = {
  {"RESULT", RESULT},
  {"A",      A},
  {"B",      B},
  {"C",      C},
  {"D",      D},
  {"E",      E},
  {"F",      F},
  {"G",      G},
  {"H",      H},
  {"I",      I},
  {"J",      J},
  {"K",      K},
  {"L",      L},
  {"M",      M},
  {"N",      N},
  {"O",      O},
  {"P",      P},
  {"SP",     SP},
  {"BP",     BP},
  {"PC",     PC},
};

char *hasm_source;
char *hasm_source_labels;

size_t hasm_source_size;

FILE *output_file;
char* output_file_name;

int expect_head = 0;

void load_hasm(FILE *);

label_entry** label_map = 0;
size_t labels_head = 0;
void find_labels();
void add_label(char* token, size_t current_address);

void assemble_hasm();

void check_matches_expectation(char* token);
void set_expect(int n, token_type a, token_type b);

void write_as_bin(token_type type, char* token);

void write_instruction(char* token);
void write_number(char* token);
void write_register(char* token);
void write_label(char* token);

const char* num_to_type(size_t n) {
  if (n == REGISTER)
    return "REGISTER";
  if (n == NUMBER)
    return "NUMBER";
  if (n == LABEL)
    return "LABEL";
  if (n == NONE)
    return "NONE";
  if (n == INSTRUCTION)
    return "INSTRUCTION";
  return "not valid instruction";
}

int parsed_int = 0;



token_type expect[4];

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
   
  output_file_name = malloc(hasm_file_len + 1);
  memcpy(output_file_name, argv[1], hasm_file_len - 4);
  output_file_name[hasm_file_len] = 0;
  output_file_name[hasm_file_len - 1 ] = 'n';
  output_file_name[hasm_file_len - 2 ] = 'i';
  output_file_name[hasm_file_len - 3 ] = 'b';
  output_file_name[hasm_file_len - 4 ] = 'h';
  output_file = fopen(output_file_name, "w");

  if (!output_file) {
    perror("couldn't open output file \n");
    exit(EXIT_FAILURE);
  }

  load_hasm(hasm_file);
  fclose(hasm_file);
  find_labels();
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
  hasm_source_labels = (char*)malloc(hasm_source_size + 1);
  if (!hasm_source) {
    perror("Couldn't allocate memory for hasm_souce.\n");
  }
  fread(hasm_source, hasm_source_size, 1, hasm);
  memcpy(hasm_source_labels, hasm_source, hasm_source_size);
}

void find_labels() {
  size_t current_address = 0;
  label_map = malloc(sizeof(label_entry*));
  char* token = strtok(hasm_source_labels, " \n\t");
  while (token) {
     if (token[strlen(token) - 1] == ':') {
       add_label(token, current_address);
     } else {
       ++current_address;
     }
    token = strtok(NULL, " \n\t");
  }
}

void add_label(char *token, size_t current_address) {
  size_t token_size = strlen(token);
  label_map[labels_head] = (label_entry*)malloc(token_size);
  label_map[labels_head]->label = token;
  label_map[labels_head]->label[token_size - 1] = 0;
  label_map[labels_head]->address = current_address;
  ++labels_head;
  label_map = realloc(label_map, labels_head);
  printf("label %s\ton address %3d added\n", label_map[labels_head - 1]->label, label_map[labels_head - 1]->address);
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

    // ignore labels
    if (token[strlen(token) - 1] == ':') {
      token = strtok(NULL, " \n\t");
      continue;
    }
    
    check_matches_expectation(token);
    write_as_bin(expect[expect_head], token);
    
    token = strtok(NULL, " \n\t");
  }
}

void check_matches_expectation(char* token) {
  // kill if should be instruction, but is not
  if (expect[expect_head] == INSTRUCTION) {
    for (int i = 0; i < INSTRUCTION_MAP_SIZE; ++i) {
      if (strcmp(instruction_map[i].token, token) == 0) {
	return;
      }
    }
    printf("Expected instruction, got %s\n", token);
    exit(EXIT_FAILURE);
  }

  // kill if should be number, but is not
  if (expect[expect_head] == NUMBER) {
    char* end;
    parsed_int = strtol(token, &end, 10);
    if (*end == '\0' && parsed_int >= 0 && parsed_int < 255) {
      return;
    }
    printf("Expected number, got %s\n", token);
    exit(EXIT_FAILURE);
  }

  // kill if should be register, but is not
  if (expect[expect_head] == REGISTER) {
    for (int i = 0; i < REGISTER_COUNT; ++i) {
      if (strcmp(register_map[i].token, token) == 0) {
        return;
      }
    }
    printf("Expected register, got %s\n", token);
  }
  if (expect[expect_head] == LABEL) {
     
     return;
  }
  
  printf("Expect %d, what  is not even a choice.", expect[expect_head]);
  exit(EXIT_FAILURE);
}

void write_as_bin(token_type type, char *token) {
  if (expect_head < 0 || expect_head > 3) {
    perror("Expect head is out of bounds.\n");
    exit(EXIT_FAILURE);
  }
  if (type == INSTRUCTION) {
    write_instruction(token);
  } else if (type == NUMBER) {
    fputc((uint8_t)parsed_int, output_file);
    --expect_head;
  } else if (type == REGISTER) {
    write_register(token);
  } else if (type == LABEL) {
    write_label(token);
  } else {
    perror("Invalid type.\n");
    exit(EXIT_FAILURE);
  }
}

void set_expect(int n, token_type a, token_type b) {
  if (n < 0 || n > 2) {
    printf("In set_expect, n has to be < 2 and > 0\n");
  }
  
  expect_head = n;
  expect[0] = INSTRUCTION;

  expect[1] = (n == 2) ? b : a;
  expect[2] = (n == 2) ? a : b;
}

void write_instruction(char *token) {
  for (int i = 0; i < INSTRUCTION_MAP_SIZE; ++i) {
    if (strcmp(token, instruction_map[i].token) == 0) {
      fputc((uint8_t)instruction_map[i].instruction, output_file);
      set_expect(instruction_map[i].expect, instruction_map[i].arg1, instruction_map[i].arg2);
      return;
    }
  }
  perror("Invalid instruction.\n");
  exit(EXIT_FAILURE);
}

void write_register(char *token) {
  for (int i = 0; i < REGISTER_COUNT; ++i) {
    if (strcmp(token, register_map[i].token) == 0) {
      putc((uint8_t)register_map[i].hal_register, output_file);
      --expect_head;
      return;
    }
  }
  printf("Invalid register. You used %s\n", token);
  exit(EXIT_FAILURE);
}

void write_label(char *token) {
  for (unsigned int i = 0; i < labels_head; ++i) {
    if (strcmp(token, label_map[i]->label) == 0) {
      putc((uint8_t)label_map[i]->address, output_file);
    }
  }
  --expect_head;
}
