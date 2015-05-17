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
#include "utilities.h"

int jobs = 0;
int dirs = 0;

/* 
 * Parse command line.
 * Returns the number of arguments. 
 */
int parsecmd(char * cmd, char ** args, int * bg, int * out)
{
	int i = split(args, cmd, " \n");

	if (i > 0) // Paramètre *
	{
		if (strcmp(args[i - 1], "*") == 0) 
		{
			struct dirent * dir;
			DIR * current = opendir(".");
			int j = 0;
			char * folders[100]; // Maximum 100 directories.

			while ((dir = readdir(current)) != NULL) 
			{
				if ((strcmp(dir->d_name, "..") != 0) &&
						(strcmp(dir->d_name, ".") != 0)) 
				{
					folders[j] = malloc(sizeof(char) * 50); // Taille 50 char
					strcpy(folders[j], dir->d_name);
					args[(i++) - 1] = folders[j++]; // args[j -> i] doivent être free après launch_process
					dirs++;
				}
			}

			args[(i--) - 1] = NULL;
			closedir(current);
		}
	}

	if (i > 0) // Programme en background
	{ 
		char * c = strrchr(args[i - 1], '&');

		if (strcmp(args[i - 1], "&") == 0) 
		{
			*bg = 1;
			args[i - 1] = NULL;
		} 
		else if (c != NULL) 
		{
			*bg = 1;
			*c = '\0';
		}			
	}

	if (i > 1 && strcmp(args[i-2], ">") == 0) // Redirection
		*out = 1;	

	return i;
}


void launch_process(char ** args, int * bg, int * out, int i)
{
	int pid;
	int j;

	if ((pid = fork()) < 0) 
	{
		perror("Erreur fork !");
		exit(EXIT_FAILURE);
	}

	// FILS
	else if (pid == 0)
	{
		//redirection
		if (*out == 1)
		{
			int h = open(args[i - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);	
			dup2(h, 1);
			close(h);
			args[i-2] = NULL;
		}	

		if (execvp(args[0], args) != 0)
			printf("Commande introuvable\n");

		//exit(EXIT_SUCCESS); Dafuk le fils doit s'arrêter, là il continue é_è
	} 
	else 
	{
		if (*bg == 0) 
		{
			waitpid(pid, 0, 0);
		}
		else 
		{
			++jobs;
			printf("[%d] %d\n", jobs, pid); 
		}
	}
}


int main(int argc, char * argv[]) {

	//signal(SIGINT,  SIG_IGN); /* Disable CTRL-C */
	//signal(SIGQUIT, SIG_IGN); /* Disable CTRL-\ */
	//signal(SIGTSTP, SIG_IGN); /* Disable CTRL-Z */

	char buffer[300];
	char * args[100];

	while (TRUE) 
	{
		shell_prompt();

		if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
		{
			printf("\n");
			exit(EXIT_FAILURE);
		}

		/* LIGNE VIDE */
		if(strcmp(buffer, "\n") == 0)
			continue;

		/* EXIT */
		if (strcmp(buffer, "exit\n") == 0)
		{
			printf("Goodbye ! \n");
			exit(EXIT_SUCCESS);
		}

		/* PARSING CMD */
		int bg = 0;
		int out = 0;
		int i = parsecmd(buffer , args, &bg, &out);

		if (strcmp(args[0], "cd") == 0)
		{
			if (args[1] == NULL) 
				chdir(getenv("HOME"));

			else if (chdir(args[1]) != 0) 
				printf("cd : no such files or directory\n");
		} 
		else 
		{
			/* EXEC CMD */
			launch_process(args, &bg, &out, i);
			for( ; dirs != 0 ; dirs--) free(args[i - dirs]);
		}
	}

	return EXIT_SUCCESS;
}

