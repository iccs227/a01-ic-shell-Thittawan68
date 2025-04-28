#include <string.h>
#include <stdio.h>
#define MAX_ARGS 64
 
void parse_input(char *input, char **args) { // Parse the input string into a bunch of arguments array
    char *right = strchr(input, '>');
    char *left = strchr(input, '<');

    if (right != NULL) {
        *right = '\0';
        right++;
        while (*right == ' ') {
            right++;
        }
        printf("Output redirection to: %s\n", right);
    }
    if (left != NULL) {
        *left = '\0';
        left++;
        while (*left == ' ') {
            left++;
        }
        printf("Input redirection from: %s\n", left);
    }
    int i = 0;
    args[i] = strtok(input, " \t\n");
    while (args[i] != NULL && i < MAX_ARGS - 1) {
        args[++i] = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}

void parse_input_with_spaces(char *input) { // Remove leading spaces from the input string
    char *trimmed_input = input;
    while (*trimmed_input == ' ' || *trimmed_input == '\t') {
        trimmed_input++;
    }
    memmove(input, trimmed_input, strlen(trimmed_input) + 1);
}