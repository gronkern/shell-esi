#ifndef SHELL_H
#define SHELL_H

#define BUFFER_SIZE 100
#define TOKENS_SIZE 100
#define FOLDERS_SIZE 100
#define ARR_SIZE 23
#define TRUE 1

/**
 * execv
 * execl
 * getcwd
 */


int parsecmd(char * cmd, char ** args, int * bg, int * out);
int execcmd(char ** args, int * bg, int * out, int i);
void launch_process(char ** args, int * bg, int * out, int i);

/**
 * Give other function to SIGINT
 */
void signal_handler_int(int sig);

/**
 * Wait for every children to do not become zombie
 */
void signal_handler_child(int sig);

#endif
