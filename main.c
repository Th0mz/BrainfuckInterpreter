#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

#define MAX_BUFFER 1000
#define MAX_COMMANDS 100000

int startLoop(char **pointer, int programCounter, int bracketPair);
int endLoop(char **pointer, int programCounter, int bracketPair);
typedef int (*CommandFunction)(char **pointer, int programCounter, int bracketPair);
  
  /* ---------: Command LinkedList Functions :--------- */
typedef struct command {
  /* Its the representation of a command, for a command to exist
        needs to have a Command structure defining him */

  char cmd;
  int bracketPair;
  CommandFunction cmdFunc;
  struct command *next;

}CommandList;

CommandList* newBracket(int pairPosition, char type) {
  CommandList *newBracket = (CommandList*) malloc(sizeof(CommandList));

  if (newBracket == NULL)
    parseError("Allocating memory for the new command");

  newBracket->cmd = type;
  newBracket->bracketPair = pairPosition;

  if (type == '[')
    newBracket->cmdFunc = startLoop;
  else
    newBracket->cmdFunc = endLoop; 

  return newBracket;
}

void addNewValidCommand(char cmd, CommandFunction cmdFunc, CommandList **head) {
  /* Creates a valid command */
  CommandList *newCommand = (CommandList*) malloc(sizeof(CommandList));

  if (newCommand == NULL)
    parseError("Allocating memory for the new command");
  
  newCommand->cmd = cmd;
  newCommand->bracketPair = 0;
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
int incrementPointer (char **pointer, int programCounter, int bracketPair) {
  /* Increment the data pointer (to point to the next cell to the right). */
  ++(*pointer);
  return 1; 
}

int decrementPointer (char **pointer, int programCounter, int bracketPair) {
  /* Decrement the data pointer (to point to the next cell to the left). */
  --(*pointer);
  return 1; 
}

int incrementValue (char **pointer, int programCounter, int bracketPair) {
  /* Increment (increase by one) the byte at the data pointer. */
  ++(**pointer);
  return 1; 
}

int decrementValue (char **pointer, int programCounter, int bracketPair) {
  /* Decrement (decrease by one) the byte at the data pointer. */
  --(**pointer);
  return 1; 
}

int outputValue (char **pointer, int programCounter, int bracketPair) {
  /* Output the byte at the data pointer. */
  printf("%c", **pointer);
  return 1; 
}

int inputValue (char **pointer, int programCounter, int bracketPair) {
  /* Accept one byte of input, storing its value in the byte at the data pointer. */
  **pointer = getchar();
  return 1; 
}

int startLoop(char **pointer, int programCounter, int bracketPair) { 
  /* If the byte at the data pointer is zero, then instead of moving the 
  instruction pointer forward to the next command, jump it forward to 
            the command after the matching ] command. */ 
  
  /*  |---------l----------------------l---------------| 
            14 = PC                 33 = BP
              |---------------------->
                        BP - PC + 1
  */
  int jump = 1;
  if (**pointer == 0) 
    return (bracketPair - programCounter) + 1;
  else 
    return 1; 
}

int endLoop(char **pointer, int programCounter, int bracketPair) { 
  /* If the byte at the data pointer is nonzero, then instead of moving
   the instruction pointer forward to the next command, jump it back to
           the command after the matching [ command */
  
    /*  |---------l----------------------l---------------| 
            14 = BP                 33 = PC
                <------------------------|
                        BR - PC + 1
  */
  int jump = 1;
  if (**pointer != 0)
    return (bracketPair - programCounter) + 1;
  else 
    return 1;
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
  addNewValidCommand('[', startLoop, &commandList);
  addNewValidCommand(']', endLoop, &commandList);


  /* Read and load the source code into the sourceCode vector */
  char cmd;
  CommandList* command;
  CommandList* sourceCode[MAX_COMMANDS];
  int insertPosition = 0;

  LinkedList *bracketFIFO = NULL;

  while((cmd = fgetc(fpSource)) != EOF) {
    if ((command = isValid(cmd, commandList))) {
      if (command->cmd == '[')
        addElement(insertPosition, &bracketFIFO);
      else if (command->cmd == ']') {
        int pairPosition = removeElement(&bracketFIFO);

        if (pairPosition < 0)
          parseError("Loop Error");

        sourceCode[pairPosition] = newBracket(insertPosition, '[');
        sourceCode[insertPosition] = newBracket(pairPosition, ']');

        printf("(%d, %d)", pairPosition, insertPosition);
      }
      else
        sourceCode[insertPosition] = command;

      insertPosition++;
    }
  }

  /* bracketFIFO is not going to be used anymore */
  freeList(bracketFIFO);

  /* DEBUG : Show the source code in the same line*/
  printf("Source Code : \n");
  for (int i = 0; i < insertPosition; i++) {
    printf("%c", sourceCode[i]->cmd);
  }

  printf("\n\n");

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
    int bracketPair = sourceCode[programCounter]->bracketPair;
    int jump = sourceCode[programCounter]->cmdFunc(&pointer, programCounter, bracketPair);
    programCounter += jump;
  }

  freeCommandList(commandList);
  return 0;
}