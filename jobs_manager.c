#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "icsh.h"
#include <ctype.h> 
#include <sys/wait.h>
#include <stdbool.h>

int size = 0;
int current_background = 0;
int prev_background = 0;
int to_be_printed = 0;

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

void removeNode(int id) {
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

            to_be_printed = 1;
            size--;
            return;
        }
        current = current->next;
    }
    printf("Node with ID %d not found.\n", id);
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
            printf("Node with ID %d status changed to %s.\n", id, newStatus);
            return;
        }
        current = current->next;
    } while (current != head);

    printf("Node with ID %d not found.\n", id);
}

void printList() {
    if (head->next == head) { // Check if the list is empty (only sentinel exists)
        printf("List is empty.\n");
        return;
    }

    Node *current = head->prev; // Start from the first real node
    while (current != head) { // Stop when we reach the sentinel
        printf("ID: %d, PID: %d, Command: %s, Status: %s\n",
               current->id, current->pid, current->command, current->status);
        if (strcmp(current->status, "Done") == 0) {
            int idToRemove = current->id;
            current = current->prev; // Move to the next node before removing
            removeNode(idToRemove);
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
    int id = atoi(input); // Convert the job ID to an integer

    // Search for the job in the list
    Node *current = head;
    do {
        if (current->id == id) {
            int status;
            foreground_pid = current->pid; // Set the foreground process ID
            kill(current->pid, SIGCONT); // Send SIGCONT to the process
            waitpid(current->pid, &status, WUNTRACED); 
            foreground_pid = -1;
            exit_code = checking_exit_code(status);
            return 1;
        }
        current = current->next;
    } while (current != head);

    return 1;
}

int add_or_update_job(int pid, char *command, char *status) {
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
void update_to_be_printed() {
    to_be_printed = 1;
}

bool job_is_done(){
    if (to_be_printed == 1){
        return true;
    }
    return false;
}

void print_done_jobs(){
    if (to_be_printed == 1){
        Node *current = head->next; 
        while (current != head) { 
            if (strcmp(current->status, "Done") == 0) {
                printf("ID: %d, PID: %d, Command: %s, Status: %s\n",
                    current->id, current->pid, current->command, current->status);
                int idToRemove = current->id;
                current = current->next; // Move to the next node before removing
                removeNode(idToRemove);
            } else {
                current = current->next;
            }
        }
    }
    to_be_printed = 0;
}