#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <sys/wait.h>

#include "icsh.h"

#define MAX_LINE 1024   
#define MAX_ARGS 64

pid_t foreground_pid = -1; // Global variable to store the foreground process ID

int new_process(char *input) { // Create a new process to execute the command
    char *args[MAX_LINE];
    strcpy(last_command, input); 

    parse_input(input, args); // Parse the input string into arguments

    int pid = fork();
    if (pid == 0) { // Child process
        execvp(args[0], args);
        perror("Invalid command");
        exit(EXIT_FAILURE);
    } else if (pid < 0) { // Fork failed
        perror ("Fork failed"); 
        return 0;
    } else { // Parent process
        int status;
        foreground_pid = pid; // Store the foreground process ID
        waitpid(pid, &status, WUNTRACED); // Wait for the child process to finish 
        foreground_pid = -1; // Reset the foreground process ID
        if (WIFEXITED(status)) {
            exit_code = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            int signal_number = WTERMSIG(status);
            exit_code = 128 + signal_number; 
        } else if (WIFSTOPPED(status)) {
            exit_code = WSTOPSIG(status);
        }
        return 1;
    }
}