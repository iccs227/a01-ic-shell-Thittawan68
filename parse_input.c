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
        //printf("%s", args[i]);
        args[++i] = strtok(NULL, ";");
    }
    args[i] = NULL;
}