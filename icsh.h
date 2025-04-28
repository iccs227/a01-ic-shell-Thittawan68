#ifndef ICHS_H
#define ICHS_H
#include <signal.h>
#include <sys/types.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

extern char last_command[MAX_LINE];  // Global variable to store the last command
extern pid_t foreground_pid; /// Global variable to store the foreground process ID
extern int exit_code; // Global variable to store the exit code of the last command

// parse_input function
void parse_input(char *input, char **args, char **input_file, char **output_file);
void parse_input_with_spaces(char *input);

// mode functions
int normal_mode(char *input);
int script_mode(char *input);

// built-in functions
int echo(char *input);
int view(char *input);
int exit_shell(char *input);

// process functions
int new_process(char *input);

// signal handler functions
void handle_sigint();
void handle_sigtstp();
#endif