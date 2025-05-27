#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "icsh.h"
#include "jobs_manager.h"


// Check if the input string contains redirection operators
bool is_redirected(char *input) {
    return (strchr(input, '>') != NULL || strchr(input, '<') != NULL); 
}

bool is_background_process(char *input) { // Check if the command should run in the background
    char *ampersand = strchr(input, '&');
    if (ampersand != NULL) {
        char argss[MAX_LINE];
        char *input_file[MAX_LINE];
        char *output_file[MAX_LINE];
        get_command_before_redirection(input, argss, input_file, output_file);
        if (input_file[0] != NULL || output_file[0] != NULL) {
            printf("Executing command without redirection: %s\n", argss);
            return 1;
        }        
        redirecting(*input_file, *output_file); 
        *ampersand = '\0'; // Remove the '&' character from the input string
        return 1; // Background process
    }
    return 0; // Foreground process
}
int handle_builtin_with_redirection(char *input) {
    char argss[MAX_LINE];
    char *input_file[MAX_LINE];
    char *output_file[MAX_LINE];
    get_command_before_redirection(input, argss, input_file, output_file);
    if (input_file[0] == NULL && output_file[0] == NULL) {
        // If there is no redirection, just execute the command
        printf("Executing command without redirection: %s\n", argss);
        return 1;
    }    
    redirecting(*input_file, *output_file); 
    int result = command_factory(argss); // Handle redirection for built-ins
    if (saved_stdout != -1) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
        saved_stdout = -1;
    }
    if (saved_stdin != -1) {
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdin);
        saved_stdin = -1;
    }
    return result;
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
    } 
    if (is_background_process(input)) { // Check if the command should run in the background 
            return background_process(input);
    }
    if (strstr(input, "!!") != NULL){    
        if (is_redirected(input)) { // Check if the command contains redirection operators
            return handle_builtin_with_redirection(input);
        }     
        return view(input);
    } else if (strncmp(input, "echo ", 5) == 0) {
        if (is_redirected(input)) { // Check if the command contains redirection operators
            return handle_builtin_with_redirection(input);
        } 
        return echo(input);
    } else if (strncmp(input, "exit", 4) == 0) {
        if (is_redirected(input)) { // Check if the command contains redirection operators
            return handle_builtin_with_redirection(input);
        } 
        return exit_shell(input);
    } else if (strncmp(input, "jobs", 4) == 0) {
        strcpy(last_command, input);
        if (is_redirected(input)) { // Check if the command contains redirection operators
            return handle_builtin_with_redirection(input);
        } 
        return print_jobs();
    } else if (strncmp(input, "fg", 2) == 0) {
        if (is_redirected(input)) { // Check if the command contains redirection operators
            return handle_builtin_with_redirection(input);
        } 
        return bring_to_foreground(input); 
    } else if (strncmp(input, "bg", 2) == 0) {
        if (is_redirected(input)) { // Check if the command contains redirection operators
            return handle_builtin_with_redirection(input);
        } 
        return continue_background(input); 
    } else {
        return new_process(input); // Not a built-in command
    }
}