#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>
#include <stdio.h>

typedef enum {
  WRONG_ARGUMENT_COUNT,
  CANT_OPEN_FILE,
  FILE_TOO_BIG,
  MALLOC_ERROR,
  DIDNT_EXIT_PROGRAM,
  SUCCESS,
  ILLEGAL_INSTRUCTION
} error_type;

void hal_exit(error_type et);

#endif
