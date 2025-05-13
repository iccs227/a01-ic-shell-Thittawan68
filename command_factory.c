#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "icsh.h"
#include "jobs_manager.h"


// Check if the input string contains redirection operators
bool is_redirected(char *input) {
    return (strchr(input, '>') != NULL || strchr(input, '<') != NULL); 
}

bool is_background_process(char *input) { // Check if the command should run in the background
    char *ampersand = strchr(input, '&');
    if (ampersand != NULL) {
        *ampersand = '\0'; // Remove the '&' character from the input string
        return 1; // Background process
    }
    return 0; // Foreground process
}

/*
This function modify "last_command" variable, so it should be used with caution
*/
// Reads input and executes commands
int command_factory(char *input) { 
    input[strcspn(input, "\n")] = '\0'; // Remove newline character
    parse_input_with_spaces(input); // Remove leading spaces

    if (strstr(input, ";") != NULL){
        return chain_mode(input); 
    } else if (is_redirected(input)) {
        return new_process(input); // Handle redirection
    }   else if (strstr(input, "!!") != NULL){
        return view(input);
    } else if (strncmp(input, "echo ", 5) == 0) {
        return echo(input);
    } else if (strncmp(input, "exit", 4) == 0) {
        return exit_shell(input);
    } else if (strncmp(input, "jobs", 4) == 0) {
        strcpy(last_command, input);
        return print_jobs();
    } else if (strncmp(input, "fg", 2) == 0) {
        return bring_to_foreground(input); 
    } else if (strncmp(input, "bg", 2) == 0) {
        return continue_background(input); 
    } else {
        if (is_background_process(input)) { // Check if the command should run in the background 
            return background_process(input);
        } else {
            return new_process(input); // Not a built-in command
        }
    }
}