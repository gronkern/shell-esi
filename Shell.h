#define BUFFER_SIZE 100
#define ARR_SIZE 10
#define TRUE 1

/**
 * execv
 * execl
 * getcwd
 */

void shellPrompt();
int parsecmd(char * cmd, char ** args, int * bg, int * out);
int execcmd(char ** args, int * bg, int * out, int i);
