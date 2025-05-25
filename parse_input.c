#include <string.h>
#include <stdio.h>
#include "icsh.h"
 
void parse_input(char *input, char **args, char **input_file, char **output_file) { // Parse the input string into a bunch of arguments array
    *input_file = NULL;
    *output_file = NULL;

    char *right = strchr(input, '>');
    char *left = strchr(input, '<');

    if (right != NULL) {
        *right = '\0'; // Split the string at the '>' character
        *output_file = strtok(right + 1, " \t\n"); // Get the output file name
    }
    if (left != NULL) {
        *left = '\0'; // Split the string at the '<' character
        *input_file = strtok(left + 1, " \t\n"); // Get the input file name
    }
    
    int i = 0;
    args[i] = strtok(input, " \t\n"); // Get the first argument
    while (args[i] != NULL && i < MAX_LINE - 1) { // Get split the rest of the arguments
        args[++i] = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}

// Remove leading spaces from the input string
void parse_input_with_spaces(char *input) { 
    char *trimmed_input = input;
    while (*trimmed_input == ' ' || *trimmed_input == '\t') { // Skip leading spaces
        trimmed_input++;
    }
    memmove(input, trimmed_input, strlen(trimmed_input) + 1); // Move the trimmed string to the beginning of the input
}

void parse_input_for_chain(char *input, char **args) { 
    int i = 0;
    args[i] = strtok(input, ";"); // Get the first argument
    while (args[i] != NULL && i < MAX_LINE - 1) { // Get split the rest of the arguments
        args[++i] = strtok(NULL, ";");
    }
    args[i] = NULL;
}

void parse_double_bash(char *new_chain_command) { 
    
    // Replace "!!" in temp_chain_command with last_command
    char *pos = strstr(new_chain_command, "!!"); // Find the position of "!!"
    char replaced_command[MAX_LINE] = ""; // Buffer to store the updated command
    strncat(replaced_command, new_chain_command, pos - new_chain_command); // Copy the part before "!!"
    strcat(replaced_command, last_command); // Append the last command
    strcat(replaced_command, pos + 2); // Append the part after "!!"
    strcpy(new_chain_command, replaced_command); // Update temp_chain_command
}


void get_command_before_redirection(const char *input, char *command, char **input_file, char **output_file) {
    *input_file = NULL;
    *output_file = NULL;

    char *right = strchr(input, '>');
    char *left = strchr(input, '<');

    if (right != NULL) {
        *right = '\0'; // Split the string at the '>' character
        *output_file = strtok(right + 1, " \t\n"); // Get the output file name
    }
    if (left != NULL) {
        *left = '\0'; // Split the string at the '<' character
        *input_file = strtok(left + 1, " \t\n"); // Get the input file name
    }

    const char *redir = strpbrk(input, "><");
    if (redir) {
        size_t len = redir - input;
        strncpy(command, input, len);
        command[len] = '\0';
    } else {
        strcpy(command, input);
    }
}