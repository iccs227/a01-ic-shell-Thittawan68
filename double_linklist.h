#ifndef DOUBLE_LINKLIST_H
#define DOUBLE_LINKLIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure of Node
typedef struct Node {
    int id;
    int pid;
    char *command;
    char *status;
    struct Node *prev;
    struct Node *next;
} Node;


// linked list functions
int addFirst(int pid, const char *command, const char *status);
void removeNode(int id);
void changeStatus(int id, const char *newStatus);
void printList();
int bring_to_foreground(char *input);
#endif