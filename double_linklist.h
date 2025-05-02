#ifndef DOUBLE_LINKLIST_H
#define DOUBLE_LINKLIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
void removeNode_by_id(int id);
void removeNode_by_pid(int pid);
void changeStatus(int id, const char *newStatus);
void printList();
int bring_to_foreground(char *input);
int add_or_update_job(int pid, const char *command, const char *status);
char* get_command_by_pid(int pid);
bool job_is_done();
int get_size();
void update_to_be_printed();
int update_jobs_status(int pid, char *status);
void print_done_jobs();
#endif