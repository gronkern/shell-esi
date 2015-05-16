#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include "Shell.h"

int bgcpt = 0;

void shellPrompt() 
{
	printf("[Shell]:%s$ ", getcwd(NULL, 0));
}

int parsecmd(char * cmd, char ** args, int * bg, int * out) {
	int i = 0;
	char * c;
	DIR * current;
	struct dirent * dir;
	char * name[100];
	int j =0;
	args[i] = strtok(cmd, " \n");

	while (args[i] != NULL) {
		i++;
		args[i] = strtok(NULL, " \n");
	}

	if (i > 0 ) {
		if (strcmp(args[i-1], "*") == 0) {
			current = opendir(".");
			while ((dir = readdir(current)) != NULL) {
				if ((strcmp(dir->d_name, "..") != 0) && (strcmp(dir->d_name, ".") != 0)) {

					name[j] = malloc(sizeof(char) *50);
					strcpy(name[j], dir->d_name);
					args[i-1] = name[j];
					i++;
					j++;
				}
			}
			args[i-1] = NULL;
			i--;	
			closedir(current);


		}
	}
	   if (i > 0) {	

	   c = strrchr(args[i-1], '&');


	   if (strcmp(args[i-1], "&") == 0) {
	 *bg = 1;
	 args[i-1] = NULL;

	 } else if (c != NULL) {
	 *bg = 1;
	 *c = '\0';
	 }			
	 }
	   if (i > 1) {
	   if (strcmp(args[i-2], ">") == 0) {
	 *out = 1;	
	 }

	 }

	return i;
}

int execcmd(char ** args, int * bg, int * out, int i) {

	int h;
	int pid = fork();
	int j;
	// ERREUR
	if (pid < 0) 
		printf("Erreur fork : %s\n", strerror(errno));

	// FILS
	else if (pid == 0) {

		//redirection
		   if (*out == 1) {
		   h = open(args[i-1], O_WRONLY | O_CREAT | O_TRUNC, 0644);	
		   dup2(h, 1);
		   close(h);
		   args[i-2] = NULL;
		   }	

		if (execvp(args[0], args) != 0)
			printf("Commande introuvable\n");


		return 0;

		//PERE
	} else {
		if (*bg == 0) {
			waitpid(pid, 0, 0);
		} else {
			bgcpt++;
			printf("[%d] %d\n", bgcpt, pid); 
		}
		return 1;
	}

}


int main(int argc, char * argv[]) {

	char buffer[300];
	char * args[100];
	int bg;
	int out;
	int i;
	while (TRUE) {
		bg = 0;
		out = 0;
		shellPrompt();
		if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
			printf("\n");
			exit(1);
		}

		/* LIGNE VIDE */
		if(strcmp(buffer, "\n") == 0)
			continue;

		/* EXIT */
		if (strcmp(buffer, "exit\n") == 0) {
			printf("Goodbye ! \n");
			exit(0);
		}



		/* PARSING CMD */
		i = parsecmd(buffer , args, &bg, &out);


		if (strcmp(args[0], "cd") == 0) {
			if (args[1] == NULL) 
				chdir(getenv("HOME"));
			else if (chdir(args[1]) != 0) 
				printf("cd : no such files or directory\n");

		} else {

			/* EXEC CMD */
			if (execcmd(args, &bg, &out, i) == 0)
				exit(0);

		}
	}

	return EXIT_SUCCESS;
}
