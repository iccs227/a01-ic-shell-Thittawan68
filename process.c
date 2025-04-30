#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h> 
#include <sys/wait.h>
#include <fcntl.h>

#include "icsh.h"
#include "double_linklist.h"

#define MAX_LINE 1024   
#define MAX_ARGS 64

pid_t foreground_pid = -1; // Global variable to store the foreground process ID

void redirecting(char *input_file, char *output_file) { // Redirect input and output files
    if (input_file != NULL) { 
        int file = open(input_file, O_RDONLY); // Redirect input, read only
        if (file < 0) {
            perror("Error opening input file");
            exit(EXIT_FAILURE);
        }
        dup2(file, STDIN_FILENO);
        close(file);
    }
    if (output_file != NULL) {
        int file = open(output_file, O_WRONLY | O_CREAT, 0777); // Redirect output file, write only or create if not exist
        if (file < 0) {
            perror("Error opening output file");
            exit(EXIT_FAILURE);
        }
        dup2(file, STDOUT_FILENO); 
        close(file);
    }
}
int checking_exit_code(int status) { // Check the exit code of the child process
    if (WIFEXITED(status)) {    // Child process terminated normally
        return WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) { // Child process terminated by a signal
        int signal_number = WTERMSIG(status);
        return 128 + signal_number; 
    } else if (WIFSTOPPED(status)) { // Child process stopped by a signal
        return WSTOPSIG(status);
    }
    return -1; // Unknown status
}

int new_process(char *input, char *last_command) { // Create a new process to execute the command
    char *args[MAX_LINE];
    char *input_file[MAX_LINE];
    char *output_file[MAX_LINE];

    strcpy(last_command, input); // Store the last command

    parse_input(input, args, input_file, output_file); // Parse the input string into arguments  

    int pid = fork();
    if (pid == 0) { // Child process
        redirecting(*input_file, *output_file); // Redirect input and output files if needed
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
        exit_code = checking_exit_code(status); // Check the exit code of the child process
        return 1;
    }
}

int background_process(char *input, char *last_command) { // Create a new process to execute the command in the background
    char *args[MAX_LINE];
    char *input_file[MAX_LINE];
    char *output_file[MAX_LINE];

   strcpy(last_command, input); // Store the last command

    parse_input(input, args, input_file, output_file); // Parse the input string into arguments

    int pid = fork();
    if (pid == 0) { // Child process
        redirecting(*input_file, *output_file); // Redirect input and output files if needed
        setpgid(0, 0); // Set the process group ID to the child process ID
        execvp(args[0], args);
        perror("Invalid command");
        exit(EXIT_FAILURE);
    } else if (pid < 0) { // Fork failed        
        perror ("Fork failed"); 
        return 0;
    } else { // Parent process
        setpgid(pid, pid); // Set the process group ID to the child process ID
        addFirst(pid, last_command, "Running"); // Add the background process to the linked list
        printf("Background process started with PID: %d\n", pid);
        return 1;
    }
}