/* ICCS227: Project 1: icsh
 * Name: Thittawan Piyasuwannkit
 * StudentID: 6680569
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>
 
 #define MAX_CMD_BUFFER 255
 
 char last_command[MAX_CMD_BUFFER] = "";
 
 
 int handle_builtin(char *input) {
     
     input[strcspn(input, "\n")] = 0;
 
     if (strncmp(input, "echo ", 5) == 0) {
         printf("%s\n", input + 5);
         return 1;
     } else if (strcmp(input, "!!") == 0) {
         if (strlen(last_command) == 0) {
             return 1; 
         }
         printf("%s\n", last_command);
         strcpy(input, last_command);
         return handle_builtin(input);
     } else if (strncmp(input, "exit", 4) == 0) {
         int code = 0;
         if (strlen(input) > 5) {
             code = atoi(input + 5) % 256; 
         } else {
             fprintf(stderr, "exit: %s: please provide exit code\n", input + 5);
             return 1;
         }
         if (code < 0 || code > 255) {
             fprintf(stderr, "exit: %s: wrong code\n", input + 5);
             return 1;
         } else {
             printf("bye\n");
             exit(code);
         }
     }
     return 0; 
 }
 
 int main(int argc, char *argv[]) {
     char input[MAX_CMD_BUFFER];
 
     while (1) {
         printf("icsh $ ");
 
         if (!fgets(input, MAX_CMD_BUFFER, stdin)) {
             break;
         }
 
         if (input[0] == '\n') {
             continue; 
         }
 
         if (handle_builtin(input)) {
             strcpy(last_command, input);
             continue;
         }
 
         printf("bad command\n");
         strcpy(last_command, input);
     }
 
     return 0;
 }
