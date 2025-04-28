#include <string.h>
#include <stdio.h>
#define MAX_ARGS 64
 
void parse_input(char *input, char **args) {
    int i = 0;
    args[i] = strtok(input, " \t\n");
    while (args[i] != NULL && i < MAX_ARGS - 1) {
        args[++i] = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}

void parse_input_with_spaces(char *input) {
    char *trimmed_input = input;
    while (*trimmed_input == ' ' || *trimmed_input == '\t') {
        trimmed_input++;
    }
    memmove(input, trimmed_input, strlen(trimmed_input) + 1);
}