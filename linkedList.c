#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

void parseError(char *errorMessage) {
  fprintf(stderr, "Error: %s\n", errorMessage);
  exit(EXIT_FAILURE);  
}

void addElement(int value, LinkedList **head) {
    /* Add a new element to the begining of the linked 
             list passed by argument */

    LinkedList *newNode = (LinkedList*) malloc(sizeof(LinkedList));

    if (newNode == 0)
        parseError("Allocating memory for the new command");

    newNode->value = value;
    newNode->next = (*head);

    (*head) = newNode;
}

int removeElement(LinkedList **head) {
    /* Removed the last element added to the 
       linked list (the head element) */

    if ((*head) == NULL)
        return -1;

    LinkedList *node = (*head);
    int removedValue = node->value;
    
    (*head) = (*head)->next;

    free(node);
    return removedValue;
}

void freeList(LinkedList *head) {
    /* Frees the memory allocated for the linked list */
    LinkedList *node;
    while (head != NULL) {
        node = head;
        head = head->next;

        free(node);
    }
}