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
int parsecmd(char * cmd, char ** tokens, int * bg, int * out)
{
	int i = split(tokens, cmd, " \n");

	if (i > 0) // Paramètre *
	{
		//if (strcmp(tokens[i - 1], "*") == 0) 
		int star_pos;
		if (star_pos = find_first((const char **) tokens, i, "*") != -1);
		{
			struct dirent * dir;
			DIR * current = opendir(".");
			//int j = 0;
			char * folders[100]; // Maximum 100 directories.

			while ((dir = readdir(current)) != NULL) 
			{
				if ((strcmp(dir->d_name, "..") != 0) &&
						(strcmp(dir->d_name, ".") != 0)) 
				{
					//if(shift_one(tokens, 
					folders[dirs] = malloc(sizeof(char) * 50); // Taille 50 char
					strcpy(folders[dirs], dir->d_name);
					//tokens[i - 1] = folders[j]; // tokens[j -> i] doivent être free après launch_process
					//i++;
					//j++;
					dirs++;
				}
			}
			
			// Pas trop sur, à tester
			shift_one(tokens, star_pos, i + dirs, dirs);
			int j = 0;
			for ( ; j < dirs; ++j)
				tokens[star_pos + j] = folders[j];	
			

			tokens[i + dirs - 2] = NULL;
			closedir(current);
		}
	}

	if (i > 0) // Programme en background
	{ 
		char * c = strrchr(tokens[i - 1], '&');

		if (strcmp(tokens[i - 1], "&") == 0) 
		{
			*bg = 1;
			tokens[i - 1] = NULL;
		} 
		else if (c != NULL) 
		{
			*bg = 1;
			*c = '\0';
		}			
	}

	if (i > 1 && strcmp(tokens[i-2], ">") == 0) // Redirection
		*out = 1;	

	return i;
}


void launch_process(char ** tokens, int * bg, int * out, int i)
{
	int pid;
	int j;

	if ((pid = fork()) < 0) 
		shell_exit_error("Erreur fork !");

	// FILS
	else if (pid == 0)
	{
		//redirection
		if (*out == 1)
		{
			int h = open(tokens[i - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);	
			dup2(h, 1);
			close(h);
			tokens[i-2] = NULL;
		}	

		if (execvp(tokens[0], tokens) != 0)
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

	char buffer[BUFFER_SIZE];
	char * tokens[TOKENS_SIZE];

	while (TRUE) 
	{
		shell_prompt();

		if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) // E.O.F. catching C^d
			shell_exit(NULL, EXIT_SUCCESS);

		if (strcmp(buffer, "exit\n") == 0)
			shell_exit("Goodbye !", EXIT_SUCCESS);

		int bg = 0;
		int out = 0;
		int i = parsecmd(buffer , tokens, &bg, &out);

		if(i == 0)
			continue;

		if (strcmp(tokens[0], "cd") == 0)
		{
			if (0 != chdir(tokens[1] == NULL 
							? getenv("HOME") 
							: tokens[1]))
				printf("cd : no such files or directory\n");
		} 
		else if (false)
		{
			//
		}
		else 
		{
			/* EXEC CMD */
			launch_process(tokens, &bg, &out, i);

			/* Clean names if * has been used */
			for( ; dirs != 0 ; dirs--) free(tokens[i - dirs]);
		}
	}

	return EXIT_SUCCESS;
}

