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

#define MAX_LINE 255   


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
        normal_mode(input); // Run in normal mode
    }
    
}
