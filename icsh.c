/* ICCS227: Project 1: icsh
 * Name: Thittawan Piyasuwannkit
 * StudentID: 6680569
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE 1024

char last_command[MAX_LINE] = "";


int normal_mode(char *input) {
    
    input[strcspn(input, "\n")] = '\0';

    if (strncmp(input, "echo ", 5) == 0) {
        printf("%s\n", input + 5);
        return 1;
    } else if (strcmp(input, "!!") == 0) {
        if (strlen(last_command) == 0) {
            return 1; 
        }
        printf("%s\n", last_command);
        strcpy(input, last_command);
        return normal_mode(input);
    } else if (strncmp(input, "exit", 4) == 0) {
        int code = 0;
        if (strlen(input) > 5) {
            code = atoi(input + 5) % 256; 
        } else {
            fprintf(stderr, "exit: %s: please provide exit code\n", input + 5);
            return 1;
        }
        if (code < 0 || code > 255) {
            fprintf(stderr, "exit: %s: wrong code\n", input + 5);
            return 1;
        } else {
            printf("bye\n");
            exit(code);
        }
    } 
    return 0; 
}

int script_mode(char *input) {
    return 0;
}

int loop(char *input) {

    while (1) {
        printf("icsh $ ");
        fflush(stdout);

        if (!fgets(input, MAX_LINE, stdin)) {
            break;
        }

        if (input[0] == '\n') {
            continue; 
        }

        if (normal_mode(input)) {
            strcpy(last_command, input);
            continue;
        } 

        printf("bad command\n");
        strcpy(last_command, input);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    char input[MAX_LINE];
    if (argc > 1) {
        script_mode(argv[1]);
    } else {
        loop(input);
    }
    
}
