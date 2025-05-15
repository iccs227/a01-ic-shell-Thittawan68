#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "icsh.h"
#include "jobs_manager.h"


int chain_mode(char *input);
char command_before_chain_command[MAX_LINE] = ""; // Global variable to store before chain command

u_int8_t exit_code = 0; // Global variable to store the exit code of the last command
int in_chain = 0; // Global variable to indicate if the command is in a chain

// Main loop for normal mode - reads input and executes commands
int normal_mode(char *input) { 
    while (1) {
        printf("icsh $ ");
        fflush(stdout);

        if (!fgets(input, MAX_LINE, stdin)) { // Read input from stdin
            break;
        }

        if (input[0] == '\n') { // If user presses Enter without input
            if (job_is_done()) { // Check if any background jobs are done
                print_done_jobs(); // Print done jobs
            } else if (background_exit_printed == 1) {  
                print_exit_jobs(); 
            }
            continue; 
        }
        
        if (command_factory(input)) {  //Normal input then do normal mode
            if (job_is_done()) { // Check if any background jobs are done
                print_done_jobs(); // Print done jobs
            }            
            continue;
        } 
    }
    return 0;
}

// Reads input from a script file and executes commands
int script_mode(char *input) { 
    FILE *script_file = fopen(input, "r");
    if (script_file == NULL) {
        fprintf(stderr, "Error opening script file: %s\n", input);
        return 0;
    }

    // Array to store all lines from the script file
    char commands[MAX_LINE][MAX_LINE];
    int command_count = 0;

    // Read the entire file into the commands array
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), script_file)) {
        // Skip empty lines and comments
        if (line[0] == '\n' || strncmp(line, "##", 2) == 0 || strncmp(line, "//", 2) == 0) {
            continue;
        }

        line[strcspn(line, "\n")] = '\0'; 
        strcpy(commands[command_count], line); // Store the line in the commands array
        command_count++;
    }
    fclose(script_file);

    // Process each command from the commands array
    printf("\n"); 
    for (int i = 0; i < command_count; i++) {
        if (command_factory(commands[i])) { // Execute each command
            if (job_is_done()) { 
                print_done_jobs(); 
            }
            continue;
        }
    }
    normal_mode(input); // Continue in normal mode after processing the script
    return 0;
}


/*
This function modify "in_chain" variable, so it should be used with caution
*/
// Handle command chaining
int chain_mode(char *input) { 
    in_chain = 1; // Set the in_chain flag to indicate that we are in chain mode
    strcpy(command_before_chain_command, last_command); // Store the command before chain command 

    char new_chain_command[MAX_LINE]; 
    strcpy(new_chain_command, input); // Use to change last_command at the end 

    if (strstr(new_chain_command, "!!") != NULL) { // Check if the command contains "!!"
        parse_double_bash(new_chain_command); // Replace "!!" with the last command
    }

    char for_parse_chain_command[MAX_LINE];
    strcpy(for_parse_chain_command, new_chain_command); // Use for parsing 

    printf("%s\n", new_chain_command);

    char *args[MAX_LINE];
    parse_input_for_chain(for_parse_chain_command, args); // Parse the input string into arguments

    for (int i = 0; args[i] != NULL; i++) {
        if (command_factory(args[i]) == 0) { // Execute each command in the chain
            break;
        }
    }
    strcpy(last_command, new_chain_command); // Store the last command
    in_chain = 0; // Reset the in_chain flag
    return 1;
}