#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER 1000

void parseError(char *errorMessage) {
  fprintf(stderr, "Error: %s\n", errorMessage);
  exit(EXIT_FAILURE);  
}

typedef void (*CommandFunction)(char *pointer);

typedef struct command {
  /* Its the representation of a command, for a command to exist
        needs to have a Command structure defining him */

  char cmd;
  CommandFunction cmdFunc;

}Command;


int main(int argc, char *argv[]) {

  /* -----: CHECK IF THE ARGUMENTS ARE VALID :----- */
  /* Check if the number of arguments is valid */
  if (argc != 3)
    parseError("Invalid number of arguments");

  int maxMemory = atoi(argv[2]); 
  /* Check if the program memory is a positive integer */
  if (maxMemory <= 0) 
    parseError("Memory must be a positive integer != 0");
  
  /* ------------------: COMPILE ------------------ */
  /* First argument of argv is the pathname for the brainfuck source code */
  FILE *fpSource;
  char *filename = argv[1]; 

  /* Load code */
  fpSource = fopen(filename, "r");
  if (fpSource == NULL)
    parseError("Failed to open the source code");

  /* Read source code */
  char command;
  while((command = fgetc(fpSource)) != EOF) {
    /* Check if the command is valid 
        -> True : Add it to the program buffer
        -> False Do nothing
    */
    printf("%c", command);
  }

  /* ----------------: EXECUTE CODE :---------------- */
  
  char memory[maxMemory];
  /* Clear memory */
  for (int i = 0; i < maxMemory; i++)
    memory[i] = 65 + i;

  /* Define memory pointer */
  char *pointer = &memory[0];




  
  /* Point to the next memory position ">" */
  ++pointer;
  /* Add one unity to the pointer value "+"*/
  ++*pointer;
  /* Print the memory position that the pointer is pointing to "." */
  printf("%c", *pointer);

  return 0;
}