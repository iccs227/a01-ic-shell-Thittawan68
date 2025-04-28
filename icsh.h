#ifndef ICHS_H
#define ICHS_H
#include <signal.h>
#include <sys/types.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

extern char last_command[MAX_LINE]; 
extern pid_t foreground_pid;
extern int exit_code;

void parse_input(char *input, char **args);
void parse_input_with_spaces(char *input);


int normal_mode(char *input);
int script_mode(char *input);
int echo(char *input);
int view(char *input);
int exit_shell(char *input);
int new_process(char *input);
void handle_sigint();
void handle_sigtstp();
#endif