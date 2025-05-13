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


// Modify the structure of JobList
int addFirst(int pid, const char *command, const char *status);
int add_or_update_job(int pid, const char *command, const char *status);
void changeStatus(int id, const char *newStatus);
int update_jobs_status(int pid, char *status);

//Remove Node from given parameter
void removeNode_by_id(int id);
void removeNode_by_pid(int pid);


//fg and bg functions
int bring_to_foreground(char *input);
int continue_background(char *input);


// Get important information
char* get_command_by_pid(int pid);
int get_size();


// Relate to printing
void printList();
void print_done_jobs();
void print_exit_jobs();
void update_to_be_printed();
bool job_is_done();

//Kill all jobs
void kill_all_jobs();

#endif