#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "icsh.h"
#include "double_linklist.h"

#define MAX_LINE 1024   
#define MAX_ARGS 64

// char last_command[MAX_LINE] = "";

int echo(char *input, char *last_command){
    
    if (strcmp(input + 5, "$?") == 0) {
        printf("%d\n", exit_code);
        exit_code = 0;
        strcpy(last_command, input);
        return 1;
    }
    strcpy(last_command, input);
    printf("%s\n", input + 5);
    return 1;
}

int view(char *input, char *last_command){
    if (strlen(last_command) == 0) {
        return 1; 
    }
    if (strlen(input) > 2) { // Check if there is anything after "!!"
        strcat(last_command, input + 2); // Append the extra part to the last command just like how terminal work
    }
    printf("%s\n", last_command);
    strcpy(input, last_command);
    return normal_mode(input, last_command);
}

int exit_shell(char *input, char *last_command){
    strcpy(last_command, input);
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

int print_jobs(){
    printList();
    return 1;
}
