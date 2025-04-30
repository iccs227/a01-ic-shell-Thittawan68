#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int size = 0;
int current_background = 0;
int prev_background = 0;

// Define the structure of Node
typedef struct Node {
    int id;
    int pid;
    char *command;
    char *status;
    struct Node *prev;
    struct Node *next;
} Node;

Node *head = NULL;

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
    return newNode;
}

// Add a node to the beginning of the circular doubly linked list
void addFirst(int pid, const char *command, const char *status) {
    Node *newNode = createNode(pid, command, status);

    if (head == NULL) {
        // If the list is empty, point the new node to itself
        newNode->next = newNode;
        newNode->prev = newNode;
        head = newNode;
    } else {
        Node *tail = (head)->prev; // Get the last node (tail)

        // Insert the new node at the beginning
        newNode->next = head;
        newNode->prev = tail;
        (head)->prev = newNode;
        tail->next = newNode;

        head = newNode; // Update the head to the new node
    }
}

// Remove a node by PID from the circular doubly linked list
void removeNode(int id) {
    if (head == NULL) {
        printf("List is empty.\n");
        return;
    }

    Node *current = head;

    // Traverse the list to find the node with the given PID
    do {
        if (current->id == id) {
            if (current->next == current && current->prev == current) {
                // If there's only one node in the list
                head = NULL;
            } else {
                // Update the links of the neighboring nodes
                current->prev->next = current->next;
                current->next->prev = current->prev;

                // Update the head if the node to be removed is the head
                if (head == current) {
                    head = current->next;
                }
            }

            // Free the memory for the node
            free(current->command);
            free(current->status);
            free(current);
            size--;
            printf("Node with ID %d removed.\n", id);
            return;
        }
        current = current->next;
    } while (current != head);

    printf("Node with ID %d not found.\n", id);
}

// Change the status of a node by PID
void changeStatus(int id, const char *newStatus) {
    if (head == NULL) {
        printf("List is empty.\n");
        return;
    }

    Node *current = head;

    // Traverse the list to find the node with the given PID
    do {
        if (current->id == id) {
            free(current->status); // Free the old status string
            current->status = strdup(newStatus); // Duplicate the new status string
            printf("Node with ID %d status changed to %s.\n", id, newStatus);
            return;
        }
        current = current->next;
    } while (current != head);

    printf("Node with ID %d not found.\n", id);
}

// Print the circular doubly linked list
void printList() {
    if (head == NULL) {
        printf("List is empty.\n");
        return;
    }

    Node *current = head;
    do {
        printf("ID: %d, PID: %d, Command: %s, Status: %s\n",
               current->id, current->pid, current->command, current->status);
        current = current->prev;
    } while (current != head);
}

