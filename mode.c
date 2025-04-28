#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "icsh.h"

#define MAX_LINE 1024   
#define MAX_ARGS 64

int exit_code = 0; // Global variable to store the exit code of the last command

// Check if the input string contains redirection operators
bool is_redirected(char *input) {
    return (strchr(input, '>') != NULL || strchr(input, '<') != NULL); 
}

// Reads input and executes commands
int normal_mode(char *input) { 
    input[strcspn(input, "\n")] = '\0'; // Remove newline character
    parse_input_with_spaces(input); // Remove leading spaces
    if (is_redirected(input)) {
        return new_process(input);
    }
    if (strncmp(input, "echo ", 5) == 0) {
        return echo(input);
    } else if (strncmp(input, "!!", 2) == 0) {
        return view(input);
    } else if (strncmp(input, "exit", 4) == 0) {
        return exit_shell(input);
    } else {
        return new_process(input); // Not a built-in command
    }
}

// Reads input from a script file and executes commands
int script_mode(char *input) { 
    FILE *script_file = fopen(input, "r");
        if (script_file == NULL) {
            fprintf(stderr, "Error opening script file: %s\n", input);
            return 0;
        }
        char line[MAX_LINE];
        while (fgets(line, sizeof(line), script_file)) {
            if (line[0] == '\n' || strncmp(line, "##", 2) == 0 || strncmp(line, "//", 2) == 0) { // Skip empty lines and comments
                continue; 
            }

            line[strcspn(line, "\n")] = '\0'; 
            
            if (normal_mode(line)) {
                continue;
            }
        }
        fclose(script_file);

    return 0;
}