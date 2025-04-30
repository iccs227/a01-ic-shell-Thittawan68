/* ICCS227: Project 1: icsh
 * Name: Thittawan Piyasuwannkit
 * StudentID: 6680569
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <sys/wait.h>

#include "icsh.h"

#define MAX_LINE 1024   
#define MAX_ARGS 64


// Main loop for normal mode - reads input and executes commands
int normal_loop(char *input, char *last_command) { 
    while (1) {
        printf("icsh $ ");
        fflush(stdout);

        if (!fgets(input, MAX_LINE, stdin)) { // Read input from stdin
            break;
        }

        if (input[0] == '\n') {
            continue; 
        }

        if (normal_mode(input, last_command)) { 
            continue;
        } 
    }

    return 0;
}

int main(int argc, char *argv[]) {
    char input[MAX_LINE];
    char last_command[MAX_LINE] = "";
    signal(SIGINT, handle_sigint); // Handle Ctrl+C
    signal(SIGTSTP, handle_sigtstp); // Handle Ctrl+Z
    signal(SIGCHLD, handle_sigchld); // Handle child process termination
    if (argc > 1) {
        struct stat file_stat;
        if (stat(argv[1], &file_stat) == 0 && S_ISREG(file_stat.st_mode)) { // Check if the file exists and is a regular file
            script_mode(argv[1], last_command); // Run in script mode
        }
    } else {
        normal_loop(input, last_command); // Run in normal mode
    }
    
}
