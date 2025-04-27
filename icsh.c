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


int normal_loop(char *input) {

    while (1) {
        printf("icsh $ ");
        fflush(stdout);

        if (!fgets(input, MAX_LINE, stdin)) {
            break;
        }

        if (input[0] == '\n') {
            continue; 
        }

        if (normal_mode(input)) {
            continue;
        } 
    }

    return 0;
}

int main(int argc, char *argv[]) {
    char input[MAX_LINE];
    if (argc > 1) {
        struct stat file_stat;
        if (stat(argv[1], &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
            script_mode(argv[1]);
        }
    } else {
        normal_loop(input);
    }
    
}
