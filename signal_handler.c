#include <sys/stat.h> 
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>

#include "icsh.h"

void handle_sigint() {
    if (foreground_pid > 0) {
        kill(foreground_pid, SIGINT);
        printf("\nSIGINT (Ctrl+C).\n");
    }
    else {
        printf("\n");
        printf("icsh $ ");
        fflush(stdout);
    }
}

void handle_sigtstp() {
    if (foreground_pid > 0) {
        kill(foreground_pid, SIGTSTP);
        printf("\nSIGTSTP (Ctrl+Z).\n");
    }
    else {
        printf("\n");
        printf("icsh $ ");
        fflush(stdout);
    }
}