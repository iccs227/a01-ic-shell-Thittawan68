#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "icsh.h"
#include "jobs_manager.h"


/*
Every function here except exit_shell modify "exit_code" variable, so it should be used with caution
*/
/*
Every function here except print_jobs modify "last_command" variable, so it should be used with caution
*/

char last_command[MAX_LINE] = "";

int echo(char *input){
    if (strcmp(input + 5, "$?") == 0) { // Check if the input is "echo $?"
        printf("%d\n", exit_code);
        exit_code = 0; // Reset exit code because we assume that buit-in command is 0
        strcpy(last_command, input);
        return 1;
    }
    exit_code = 0; // Reset exit code because we assume that buit-in command is 0
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

    if (in_chain == 1) { // If the command is in a chain
        strcpy(last_command, command_before_chain_command); // Copy the chain command to the result
    } 

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
    char args[MAX_LINE];
    exit_code = 0; // Reset exit code because we assume that buit-in command is 0
    printf("%s\n", result); // Print the final result
    strcpy(args, result);  // Update the input with the final result
    return command_factory(args); // Pass the updated input to normal_mode to execute it
}

int exit_shell(char *input){
    strcpy(last_command, input);
    int code = 0;
    code = atoi(input + 5) & 0xFF; // make sure the code is between 0 and 255
    kill_all_jobs(); // Kill all background jobs
    printf("bye\n");
    exit(code);
    
}

int print_jobs(){ // Print the list of jobs
    exit_code = 0; // Reset exit code because we assume that buit-in command is 0
    printList();
    return 1;
}


