#ifndef LINKED_LIST
#define LINKED_LIST

/* Linked list basic structure (node) */
typedef struct node {
    int value;
    struct node *next;
    
} LinkedList;

/* Functions */
void parseError(char *errorMessage);

void addElement(int value, LinkedList **head);
int removeElement(LinkedList **head);
void freeList(LinkedList *head);

#endif