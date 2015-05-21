#include "Shell.h"

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

#include "utilities.h"

int jobs = 0;
int dirs = 0;
char * folders[FOLDERS_SIZE]; // Maximum 100 directories.

/* 
 * Parse command line.
 * Returns the number of arguments. 
 */
int parsecmd(char * cmd, char ** tokens, int * bg, int * out)
{
	int n_tokens = split(tokens, cmd, "\t \n");

	if (n_tokens > 0) // Paramètre *
	{
		int star_pos = find_first((const char **) tokens, n_tokens, "*");
		if (star_pos != -1)
		{
			--n_tokens; // On enlève le *
			dirs = 0; // On remet dirs à 0
			struct dirent * dir;
			DIR * current = opendir(".");

			while ((dir = readdir(current)) != NULL) 
			{
				if ((strcmp(dir->d_name, "..") != 0) &&
						(strcmp(dir->d_name, ".") != 0)) 
				{
					folders[dirs] = malloc(sizeof(char) * 50); // Taille 50 char
					strcpy(folders[dirs], dir->d_name);
					++dirs;
					++n_tokens; // On rajoute un argument

				}
			}
		
			// Décalage des arguments après le *
			shift(tokens, star_pos, n_tokens, dirs);

			// On copie tous les fichiers / folders dans les tokens
			for (int j = 0; j < dirs; ++j)
				tokens[star_pos + j] = folders[j];

			tokens[n_tokens] = NULL; // Dernier argument doit être nul
			closedir(current);
		}
	}

	if (n_tokens > 0) // Programme en background
	{ 
		char * c = strrchr(tokens[n_tokens - 1], '&');

		if (strcmp(tokens[n_tokens - 1], "&") == 0) 
		{
			*bg = 1;
			tokens[n_tokens - 1] = NULL;
		} 
		else if (c != NULL) 
		{
			*bg = 1;
			*c = '\0';
		}			
	}

	if (n_tokens > 1 && strcmp(tokens[n_tokens-2], ">") == 0) // Redirection
		*out = 1;	

	return n_tokens;
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

		if (execvp(tokens[0], tokens) == -1)
			printf("Commande introuvable\n");

		exit(EXIT_SUCCESS); 
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
		int n_tokens = parsecmd(buffer , tokens, &bg, &out);
	
		if(n_tokens == 0)
			continue;
		
		if (strcmp(tokens[0], "cd") == 0)
		{
			if (chdir(tokens[1] == NULL 
							? getenv("HOME") 
							: tokens[1]) != 0)
				printf("cd : no such files or directory\n");
		} 
		else if (false)
		{
			//
		}
		else 
		{
			/* EXEC CMD */
			launch_process(tokens, &bg, &out, n_tokens);

			for(int i = 0; i < dirs; ++i) 
			{
				free(folders[i]);
				folders[i] = NULL;
			}
		}
	}

	return EXIT_SUCCESS;
}

