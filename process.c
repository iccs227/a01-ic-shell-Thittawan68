#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <sys/wait.h>

#include "icsh.h"

#define MAX_LINE 1024   
#define MAX_ARGS 64

pid_t foreground_pid = -1;

int new_process(char *input) {
    char *args[MAX_LINE];
    strcpy(last_command, input);

    parse_input(input, args);

    int pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("Invalid command");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror ("Fork failed");
        return 0;
    } else {
        int status;
        foreground_pid = pid;
        waitpid(pid, &status, WUNTRACED);
        foreground_pid = -1;
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