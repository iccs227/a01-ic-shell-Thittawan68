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
#include "double_linklist.h"

#define MAX_LINE 255   


// Main loop for normal mode - reads input and executes commands
int normal_loop(char *input) { 
    while (1) {
        printf("icsh $ ");
        fflush(stdout);

        if (!fgets(input, MAX_LINE, stdin)) { // Read input from stdin
            break;
        }

        if (input[0] == '\n') { // If user presses Enter without input
            if (job_is_done()) { // Check if any background jobs are done
                print_done_jobs(); // Print done jobs
            } else if (background_exit_printed == 1) {  
                print_exit_jobs(); 
            }
            continue; 
        }
        
        if (normal_mode(input)) {  //Normal input then do normal mode
            if (job_is_done()) { // Check if any background jobs are done
                print_done_jobs(); // Print done jobs
            }            
            continue;
        } 
    }

    return 0;
}

int main(int argc, char *argv[]) {
    char input[MAX_LINE];
    signal(SIGINT, handle_sigint); // Handle Ctrl+C
    signal(SIGTSTP, handle_sigtstp); // Handle Ctrl+Z
    signal(SIGCHLD, handle_sigchld); // Handle child process termination
    if (argc > 1) {
        struct stat file_stat;
        if (stat(argv[1], &file_stat) == 0 && S_ISREG(file_stat.st_mode)) { // Check if the file exists and is a regular file
            script_mode(argv[1]); // Run in script mode
        }
    } else {
        normal_loop(input); // Run in normal mode
    }
    
}
