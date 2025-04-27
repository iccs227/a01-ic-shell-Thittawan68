#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "icsh.h"

#define MAX_LINE 1024   
#define MAX_ARGS 64



int normal_mode(char *input) {
    input[strcspn(input, "\n")] = '\0';
    
    if (strncmp(input, "echo ", 5) == 0) {
        return echo(input);
    } else if (strcmp(input, "!!") == 0) {
        return view(input);
    } else if (strncmp(input, "exit", 4) == 0) {
        return exit_shell(input);
    } else {
        return new_process(input);
    }
}

int script_mode(char *input) {
    FILE *script_file = fopen(input, "r");
        if (script_file == NULL) {
            fprintf(stderr, "Error opening script file: %s\n", input);
            return 0;
        }
        char line[MAX_LINE];
        while (fgets(line, sizeof(line), script_file)) {
            if (line[0] == '\n' || strncmp(line, "##", 2) == 0 || strncmp(line, "//", 2) == 0) {
                continue; 
            }

            line[strcspn(line, "\n")] = '\0'; 
            
            if (normal_mode(line)) {
                continue;
            }

            printf("bad command\n");
            strcpy(last_command, line);
        }
        fclose(script_file);

    return 0;
}