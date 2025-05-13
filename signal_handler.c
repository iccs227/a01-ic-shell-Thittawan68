#include <sys/stat.h> 
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include "icsh.h"
#include "jobs_manager.h"

void handle_sigint() { // Handle Ctrl+C
    if (foreground_pid > 0) {
        kill(foreground_pid, SIGINT); // Send SIGINT (terminate) to the foreground process
        write(STDOUT_FILENO, "\n", 1); 
    }
    else {
        write(STDOUT_FILENO, "\n", 1); 
        write(STDOUT_FILENO, "icsh $ ", 7); 
        fflush(stdout); // if no foreground process, just print a new prompt and flush stdout
    }
}

void handle_sigtstp() { // Handle Ctrl+Z
    if (foreground_pid > 0) {
        char *command = get_command_by_pid(foreground_pid); // Get the command of the foreground process
        int id = get_size();
    
        if (command == NULL) { // If command is NULL, use the last command
            id = add_or_update_job(foreground_pid, last_command, "Stopped");
        }
        else {
            id = add_or_update_job(foreground_pid, command, "Stopped"); // Update the job status to "Stopped"
        }

        kill(foreground_pid, SIGTSTP); // Send SIGTSTP (stop) to the foreground process
        //weird printing to be "signal safe"
        char buffer[64];
        int len;
        if (command == NULL) {
            len = snprintf(buffer, sizeof(buffer), "\n[%d] Stopped     %s", id, last_command); // Format the output
        }
        else {
            len = snprintf(buffer, sizeof(buffer), "\n[%d] Stopped     %s", id, command); // Format the output
        }
        write(STDOUT_FILENO, buffer, len); // Print the job status
        write(STDOUT_FILENO, "\n ", 1); 
    } else {
        write(STDOUT_FILENO, "\n", 1); 
        write(STDOUT_FILENO, "icsh $ ", 7); 
        fflush(stdout); // If no foreground process, just print a new prompt and flush stdout
    }
}

/*
This function modify "background_exit_printed", so it should be used with caution
*/
void handle_sigchld() {
    int status;
    int pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0){
        exit_code = checking_exit_code(status);
        if (exit_code != 1){
            update_to_be_printed(); // tell the program there is something to print next time
            update_jobs_status(pid, "Done"); // Update the job status to "Done"
        }
        else{
            background_exit_printed = 1; // Invalid background process
        }        
    };
}
