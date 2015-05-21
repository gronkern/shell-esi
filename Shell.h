#ifndef SHELL_H
#define SHELL_H

#define BUFFER_SIZE 100
#define TOKENS_SIZE 100
#define ARR_SIZE 10
#define TRUE 1

/**
 * execv
 * execl
 * getcwd
 */

int parsecmd(char * cmd, char ** args, int * bg, int * out);
int execcmd(char ** args, int * bg, int * out, int i);
void launch_process(char ** args, int * bg, int * out, int i);

#endif
