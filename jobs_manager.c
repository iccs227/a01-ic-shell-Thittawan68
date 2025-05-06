#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "icsh.h"
#include <ctype.h> 
#include <sys/wait.h>
#include <stdbool.h>
#include <unistd.h> 

int size = 0;
int current_background = 0;
int prev_background = 0;
int to_be_printed = 0;
int background_exit_printed = 0; // Global variable to indicate if there is something to print

// Define the structure of Node
typedef struct Node {
    int id;
    int pid;
    char *command;
    char *status;
    struct Node *prev;
    struct Node *next;
} Node;

// Set up the sentinel node
Node sentinel = { .id = 0, .pid = 0, .command = NULL, .status = NULL, .prev = &sentinel, .next = &sentinel };
Node *head = &sentinel; 

// Function to create a new node with malloc
Node* createNode(int pid, const char *command, const char *status) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->pid = pid;
    newNode->command = strdup(command); // Duplicate the command string
    newNode->status = strdup(status);   // Duplicate the status string
    newNode->prev = NULL;
    newNode->next = NULL;
    size++;
    newNode->id = size; // Assign a unique ID to the node
    return newNode;
}

int addFirst(int pid, const char *command, const char *status) {
    Node *newNode = createNode(pid, command, status);

    // Insert the new node after the sentinel
    newNode->next = head->next;
    newNode->prev = head;
    head->next->prev = newNode;
    head->next = newNode;

    return newNode->id; 
}

void removeNode_by_id(int id) {
    if (head->next == head) { // Check if the list is empty (only sentinel exists)
        printf("List is empty.\n");
        return;
    }

    Node *current = head->next; // Start from the first real node

    while (current != head) { // Stop when we reach the sentinel
        if (current->id == id) {
            // Update the links of the neighboring nodes
            current->prev->next = current->next;
            current->next->prev = current->prev;

            // Free the memory for the node
            free(current->command);
            free(current->status);
            free(current);
            if (head->next == head) { 
                size = 0; // Reset size if the list is empty
            }
            to_be_printed = 1; // tell the program there is something to print next time
            return;
        }
        current = current->next;
    }
}

void removeNode_by_pid(int pid) {
    if (head->next == head) { // Check if the list is empty (only sentinel exists)
        printf("List is empty.\n");
        return;
    }

    Node *current = head->next; // Start from the first real node

    while (current != head) { // Stop when we reach the sentinel
        if (current->pid == pid) {
            // Update the links of the neighboring nodes
            current->prev->next = current->next;
            current->next->prev = current->prev;

            // Free the memory for the node
            free(current->command);
            free(current->status);
            free(current);
            if (head->next == head) { 
                size = 0; // Reset size if the list is empty
            }
            to_be_printed = 1; // tell the program there is something to print next time
            return;
        }
        current = current->next;
    }
}

// Change the status of a node by PID
void changeStatus(int id, const char *newStatus) {
    if (head == NULL) {
        printf("List is empty.\n");
        return;
    }

    Node *current = head;

    // Traverse the list to find the node with the given ID
    do {
        if (current->id == id) {
            free(current->status); 
            current->status = strdup(newStatus); 
            return;
        }
        current = current->next;
    } while (current != head);
}

void printList() {
    if (head->next == head) { // Check if the list is empty (only sentinel exists)
        return;
    }

    Node *current = head->prev; // Start from the first real node
    while (current != head) { // Stop when we reach the sentinel
        printf("[%d] %s     %s\n", current->id, current->status, current->command);
        if (strcmp(current->status, "Done") == 0) {
            int idToRemove = current->id;
            current = current->prev; // Move to the next node before removing
            removeNode_by_id(idToRemove);
        } else {
            current = current->prev;
        }
    }
}


int bring_to_foreground(char *input) {
    if (head == NULL) {
        printf("No background jobs to bring to foreground.\n");
        return 1;
    }

    strcpy(last_command, input);

    // Trim leading spaces
    while (isspace(*input)) {
        input++;
    }

    // Check if the command starts with "fg"
    if (strncmp(input, "fg", 2) != 0) {
        printf("Invalid command. Usage\n");
        return 1;
    }

    // Move past "fg" and any spaces
    input += 2;
    while (isspace(*input)) {
        input++;
    }

    // Check if the next character is '%'
    if (*input != '%') {
        printf("Invalid input format. \n");
        return 1;
    }

    // Move past '%' and extract the job ID
    input++;
    while (isspace(*input)) {
        input++;
    }
    if (!isdigit(*input)) { // Check if the input is not a digit
        printf("fg: %%%s: no such job\n", input);
        return 1;
    }
    
    int id = atoi(input); // Convert the job ID to an integer

    // Search for the job in the list
    Node *current = head;
    do {
        if (current->id == id) {
            int status;
            foreground_pid = current->pid; // Set the foreground process ID
            if (current->command[strlen(current->command) - 1] == '&') { 
                current->command[strlen(current->command) - 1] = '\0'; 
            }
            //weird printing to be "signal safe"
            int len = snprintf(NULL, 0, "%s\n", current->command); // Get length
            char *buffer = malloc(len + 1); // Allocate memory
            snprintf(buffer, len + 1, "%s\n", current->command); // Format the string
            write(STDOUT_FILENO, buffer, len); // Print the job status

            kill(current->pid, SIGCONT); // Send SIGCONT to the process
            waitpid(current->pid, &status, WUNTRACED); 
            if (WIFEXITED(status) || WIFSIGNALED(status)) {    // Child process terminated normally
                removeNode_by_pid(current->pid);
            } 
            foreground_pid = -1;
            exit_code = checking_exit_code(status);
            return 1;
        }
        current = current->next;
    } while (current != head);
    printf("fg: %%%d: no such job\n", id); // Job not found

    return 1;
}
int continue_background(char *input) {
    if (head == NULL) {
        printf("No background jobs to continue.\n");
        return 1;
    }

    strcpy(last_command, input);

    // Trim leading spaces
    while (isspace(*input)) {
        input++;
    }

    // Check if the command starts with "bg"
    if (strncmp(input, "bg", 2) != 0) {
        printf("Invalid command. Usage\n");
        return 1;
    }

    // Move past "bg" and any spaces
    input += 2;
    while (isspace(*input)) {
        input++;
    }

    // Check if the next character is '%'
    if (*input != '%') {
        printf("Invalid input format. \n");
        return 1;
    }

    // Move past '%' and extract the job ID
    input++;
    while (isspace(*input)) {
        input++;
    }
    if (!isdigit(*input)) { // Check if the input is not a digit
        printf("bg: %%%s: no such job\n", input);
        return 1;
    }
    
    int id = atoi(input); // Convert the job ID to an integer

    Node *current = head;
    do {
        if (current->id == id) {
            foreground_pid = current->pid; 
            if (current->command[strlen(current->command) - 1] != '&') { 
                strcat(current->command, "&");
            }
            //weird printing to be "signal safe"
            int len = snprintf(NULL, 0, "[%d]      %s\n", id, current->command); // Get length
            char *buffer = malloc(len + 1); // Allocate memory
            snprintf(buffer, len + 1, "[%d]      %s\n", id, current->command); // Format the string
            write(STDOUT_FILENO, buffer, len); // Print the job status
            free(buffer); // Free the allocated memory
            changeStatus(current->id, "Running");
            kill(current->pid, SIGCONT); // Send SIGCONT to the process
            foreground_pid = -1;
            return 1;
        }
        current = current->next;
    } while (current != head);
    printf("fg: %%%d: no such job\n", id); // Job not found

    return 1;
}

// if the jobs is not in the list, add it
// if the jobs is in the list, update it
int add_or_update_job(int pid, char *command, char *status) {
    //if empty list, add the first node
    if (head == NULL) { 
        return addFirst(pid, command, status);
    }

    Node *current = head;
    do {
        if (current->pid == pid) {
            free(current->command);
            free(current->status);
            char* new_command = malloc(strlen(command) + 1);
            strcpy(new_command, command);
            current->command = strdup(new_command);
            current->status = strdup(status);
            return current->id;
        }
        current = current->next;
    } while (current != head);

    return addFirst(pid, command, status);
}

// Update the status of a job by given PID
int update_jobs_status(int pid, char *status) {
    if (head == NULL) {
        return 0; 
    }

    Node *current = head;
    do {
        if (current->pid == pid) {
            free(current->status);
            current->status = strdup(status);
            return 1;
        }
        current = current->next;
    } while (current != head);

    return 0; 
}

//return the command of a job by given PID
char* get_command_by_pid(int pid) {
    if (head == NULL) {
        return NULL; 
    }

    Node *current = head;

    // Traverse the list to find the node with the given PID
    do {
        if (current->pid == pid) {
            char* command = malloc(strlen(current->command) + 1);
            strcpy(command, current->command);
            return command; 
        }
        current = current->next;
    } while (current != head);

    return NULL; // Return NULL if the node is not found
}


int get_size() {
    return size;
}

//trigger when their is a jobs done 
void update_to_be_printed() {
    to_be_printed = 1;
}

bool job_is_done(){
    if (to_be_printed == 1){
        return true;
    }
    return false;
}

// Print the list of jobs with status "Done" and remove them from the list
void print_done_jobs(){
    if (to_be_printed == 1){
        Node *current = head->next; 
        while (current != head) { 
            if (strcmp(current->status, "Done") == 0) {
                if (current->command[strlen(current->command) - 1] == '&') { 
                    current->command[strlen(current->command) - 1] = '\0'; 
                }
                printf("[%d] Done     %s\n", current->id, current->command);
                int idToRemove = current->id;
                current = current->next; // Move to the next node before removing
                removeNode_by_id(idToRemove);
            } else {
                current = current->next;
            }
        }
    }
    to_be_printed = 0; // Reset the flag after printing
}
void delete_first() {
    if (head->next == head) { // Check if the list is empty (only sentinel exists)
        printf("List is empty.\n");
        return;
    }

    Node *firstNode = head->next; // Get the first real node
    head->next = firstNode->next; // Update the sentinel's next pointer
    firstNode->next->prev = head; // Update the next node's prev pointer

    free(firstNode->command);
    free(firstNode->status);
    free(firstNode);
}

void print_exit_jobs(){
    if (background_exit_printed){
        delete_first(); // Remove the first node from the list
        char buffer[64];
        int len;
        len = snprintf(buffer, sizeof(buffer), "\n[%d] Exit 127    %s\n", size, last_command); // Format the output
        write(STDOUT_FILENO, buffer, len);
    }
    background_exit_printed = 0; // Reset the flag after printing
}