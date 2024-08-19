#include "error.h"
#include "memory.h"
#include <stdlib.h>

void hal_exit(error_type et) {  
  switch (et) {
  case WRONG_ARGUMENT_COUNT:
    perror("You have to provide bytecode file.\nExample:\n\tcommand hello_world.bc\n");
    exit(EXIT_FAILURE);
    break;
  case CANT_OPEN_FILE:
    perror("Couln't open bytecode file. Does your file exist?\n");
    exit(EXIT_FAILURE);
    break;
  case FILE_TOO_BIG:
    perror("Your program is too big.\n");
    exit(EXIT_FAILURE);
    break;
  case MALLOC_ERROR:
    perror("Couldn't allocate memory.\n");
    exit(EXIT_FAILURE);
    break;
  case DIDNT_EXIT_PROGRAM:
    perror("You didn't exit program. PC was about to overflow.\n");
    exit(EXIT_FAILURE);
    break;
  case SUCCESS:
    printf("Computer \"Hal 0.009\" successfully shuted down by OS \"%s\".", program_name);
    exit(EXIT_SUCCESS);
    break;
  case ILLEGAL_INSTRUCTION:
    printf("You tried to use illegal instruction.\n");
    exit(EXIT_FAILURE);
  default:
    perror("Unknows error.\n");
    exit(EXIT_FAILURE);
    break;
  }
}
