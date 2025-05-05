#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "icsh.h"
#include "double_linklist.h"

#define MAX_LINE 1024   
#define MAX_ARGS 64

char last_command[MAX_LINE] = "";

int echo(char *input){
    
    if (strcmp(input + 5, "$?") == 0) { // Check if the input is "echo $?"
        printf("%d\n", exit_code);
        exit_code = 0; // Reset exit code because we assume that buit-in command is 0
        strcpy(last_command, input);
        return 1;
    }
    strcpy(last_command, input);
    printf("%s\n", input + 5);
    return 1;
}

int view(char *input) {
    if (strlen(last_command) == 0) {
        return 1;
    }

    char result[MAX_LINE] = ""; // Buffer to store the final string
    char *pos = strstr(input, "!!"); // Find the position of "!!"

    if (pos != NULL) {
        // Copy the part before "!!"
        strncat(result, input, pos - input);

        // Append the last command
        strcat(result, last_command);

        // Append the part after "!!"
        strcat(result, pos + 2);
    } else {
        // If "!!" is not found, just copy the input as is
        strcpy(result, input);
    }

    printf("%s\n", result); // Print the final result
    strcpy(input, result);  // Update the input with the final result
    return normal_mode(input); // Pass the updated input to normal_mode to execute it
}

int exit_shell(char *input){
    strcpy(last_command, input);
    int code = 0;
    if (strlen(input) > 5) {
        code = atoi(input + 5) % 256; // make sure the code is between 0 and 255
    } 
    if (code < 0 || code > 255) { // Check if the code is not between 0 and 255
        fprintf(stderr, "exit: %s: wrong code\n", input + 5);
        return 1;
    } else {
        printf("bye\n");
        exit(code);
    }
}

int print_jobs(){ // Print the list of jobs
    printList();
    return 1;
}


