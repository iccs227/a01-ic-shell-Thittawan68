#include <sys/stat.h> 
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "icsh.h"
#include "double_linklist.h"

void handle_sigint() { // Handle Ctrl+C
    if (foreground_pid > 0) {
        kill(foreground_pid, SIGINT); // Send SIGINT (terminate) to the foreground process
        write(STDOUT_FILENO, "\n", 1); // Print a newline
    }
    else {
        write(STDOUT_FILENO, "\n", 1); // Print a newline
        write(STDOUT_FILENO, "icsh $ ", 7); // Print the prompt
        fflush(stdout); // if no foreground process, just print a new prompt and flush stdout
    }
}

void handle_sigtstp() { // Handle Ctrl+Z
    if (foreground_pid > 0) {
        addFirst(foreground_pid, last_command, "Running");
        kill(foreground_pid, SIGTSTP); // Send SIGTSTP (stop) to the foreground process
        write(STDOUT_FILENO, "\n", 1); // Print a newline
    }
    else {
        write(STDOUT_FILENO, "\n", 1); // Print a newline
        write(STDOUT_FILENO, "icsh $ ", 7); // Print the prompt
        fflush(stdout); // if no foreground process, just print a new prompt and flush stdout
    }
}

void handle_sigchld(int sig) {
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0){
        exit_code = checking_exit_code(status);
    };
}
