#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER 1000
#define MAX_COMMANDS 100000

void parseError(char *errorMessage) {
  fprintf(stderr, "Error: %s\n", errorMessage);
  exit(EXIT_FAILURE);  
}

typedef void (*CommandFunction)(char **pointer);
  
  /* ---------: Command LinkedList Functions :--------- */
typedef struct command {
  /* Its the representation of a command, for a command to exist
        needs to have a Command structure defining him */

  char cmd;
  CommandFunction cmdFunc;
  struct command *next;

}CommandList;

void addNewValidCommand(char cmd, CommandFunction cmdFunc, CommandList **head) {
  /* Creates a valid command */
  CommandList *newCommand = (CommandList*) malloc(sizeof(CommandList));

  if (newCommand == NULL)
    parseError("Allocating memory for the new command");
  
  newCommand->cmd = cmd;
  newCommand->cmdFunc = cmdFunc;
  newCommand->next = (*head);

  (*head) = newCommand;
}

CommandList* isValid(char command, CommandList *validCommands) {
  if (validCommands == NULL)
    return NULL;

  CommandList *currentCommand;
  for (currentCommand = validCommands; currentCommand != NULL; currentCommand = currentCommand->next)
    if (currentCommand->cmd == command)
      return currentCommand;

  return NULL;
}

void freeCommandList(CommandList *head) {
  /* Frees all the allocated blocks of the command list */
  if (head == NULL)
    return;

  CommandList *command;
  while (head != NULL) {
    command = head;
    head = head->next;

    free(command);
  }
}

  /* --------------------: Command Functions -------------------- */
void incrementPointer (char **pointer) {
  /* Increment the data pointer (to point to the next cell to the right). */
  ++(*pointer);
}

void decrementPointer (char **pointer) {
  /* Decrement the data pointer (to point to the next cell to the left). */
  --(*pointer);
}

void incrementValue (char **pointer) {
  /* Increment (increase by one) the byte at the data pointer. */
  ++(**pointer);
}

void decrementValue (char **pointer) {
  /* Decrement (decrease by one) the byte at the data pointer. */
  --(**pointer);
}

void outputValue (char **pointer) {
  /* Output the byte at the data pointer. */
  printf("%c", **pointer);
}

void inputValue (char **pointer) {
  /* Accept one byte of input, storing its value in the byte at the data pointer. */
  **pointer = getchar();
}



int main(int argc, char *argv[]) {

  /* -----: Argument Check :----- */
  /* Check if the number of arguments is valid */
  if (argc != 3)
    parseError("Invalid number of arguments");

  int maxMemory = atoi(argv[2]); 
  /* Check if the program memory is a positive integer */
  if (maxMemory <= 0) 
    parseError("Memory must be a positive integer != 0");
  
  /* ------------------: Compile ------------------ */
  /* First argument of argv is the pathname for the brainfuck source code */
  FILE *fpSource;
  char *filename = argv[1]; 

  /* Load code */
  fpSource = fopen(filename, "r");
  if (fpSource == NULL)
    parseError("Failed to open the source code");

  /* Initialize possible commands */
  CommandList *commandList = NULL;

  addNewValidCommand('>', incrementPointer, &commandList);
  addNewValidCommand('<', decrementPointer, &commandList);
  addNewValidCommand('+', incrementValue, &commandList);
  addNewValidCommand('-', decrementValue, &commandList);
  addNewValidCommand('.', outputValue, &commandList);
  addNewValidCommand(',', inputValue, &commandList);

  /* Read and load the source code into the sourceCode vector */
  char cmd;
  CommandList* command;
  CommandList* sourceCode[MAX_COMMANDS];
  int insertPosition = 0;

  while((cmd = fgetc(fpSource)) != EOF) {
    if ((command = isValid(cmd, commandList))) {
      sourceCode[insertPosition] = command;
      insertPosition++;
    }

  }

  /* ----------------: Execute Code :---------------- */
  
  char memory[maxMemory];
  /* Clear memory */
  for (int i = 0; i < maxMemory; i++)
    memory[i] = 0;

  /* Define memory pointer */
  char *pointer = &memory[0];
  int programCounter = 0;

  /* Run the brainfuck program cycle */
  printf("Running the program...\n");
  while (programCounter < insertPosition) {
    sourceCode[programCounter]->cmdFunc(&pointer);
    programCounter++;
  }

  freeCommandList(commandList);
  return 0;
}