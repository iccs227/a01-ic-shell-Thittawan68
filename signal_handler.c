#include <sys/stat.h> 
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>

#include "icsh.h"

void handle_sigint() { // Handle Ctrl+C
    if (foreground_pid > 0) {
        kill(foreground_pid, SIGINT);
        printf("\n");
    }
    else {
        printf("\n");
        printf("icsh $ ");
        fflush(stdout); // if no foreground process, just print a new prompt and flush stdout
    }
}

void handle_sigtstp() { // Handle Ctrl+Z
    if (foreground_pid > 0) {
        kill(foreground_pid, SIGTSTP);
        printf("\n");
    }
    else {
        printf("\n");
        printf("icsh $ ");
        fflush(stdout); // if no foreground process, just print a new prompt and flush stdout
    }
}