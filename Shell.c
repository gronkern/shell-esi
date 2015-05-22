#define _POSIX_SOURCE

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
#include <signal.h>

#include "utilities.h"

struct sigaction act_int;
struct sigaction act_child;
pid_t pid;
pid_t pid_shell;
int jobs = 0;
int dirs = 0;
char * folders[FOLDERS_SIZE]; // Maximum 100 directories.
int redirections[ARR_SIZE];
int size_redirec = 0;


void signal_handler_child(int sig)
{
	while(waitpid(-1, NULL, WNOHANG) > 0); // remove every zombies children
}

void signal_handler_int(int sig)
{
		if(sig = SIGINT)
			printf("\nLe processus %d a reçu un signal SIGINT\n", pid);
		else
			printf("\nAutre signal\n");
}

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

	if (n_tokens > 1) // Programme en background
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

	//if (n_tokens > 1 && strcmp(tokens[n_tokens-2], ">") == 0) // Redirection
	int pos_redirection = find_first((const char **) tokens, n_tokens, ">");
	if (n_tokens > 2 && pos_redirection != -1)
	{
		*out = 1;	
		redirections[size_redirec++] = pos_redirection;
		printf("%d  pos_redir\n", pos_redirection);
		printf("%d  size_redir\n", size_redirec);
		printf("%d  pos redir tab\n", redirections[size_redirec -1]);
	}

	return n_tokens;
}


void launch_process(char ** tokens, int * bg, int * out, int i)
{
	int j;

	if ((pid = fork()) < 0) 
		shell_exit_error("Erreur fork !");

	// FILS
	else if (pid == 0)
	{
		
		// unbind C^c 
		sigaction(SIGINT, &act_int, NULL);

		//redirection
		if (*out == 1)
		{
			//int h = open(tokens[i - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);	
			int h = open(tokens[redirections[size_redirec - 1] + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);	
			dup2(h, 1);
			close(h);
			//tokens[i-2] = NULL;
			tokens[redirections[size_redirec - 1]] = NULL;
			size_redirec--;
		}	

		if (execvp(tokens[0], tokens) == -1)
		{
			printf("Commande introuvable\n");
			kill(getpid(), SIGTERM);
		}

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

	/** Global variable init **/
	pid_shell = getpid();
    act_child.sa_handler = signal_handler_child;
	act_int.sa_handler = signal_handler_int;

	/** Tokens variables **/
	char buffer[BUFFER_SIZE];
	char * tokens[TOKENS_SIZE];


	sigaction(SIGCHLD, &act_child, NULL);
	sigaction(SIGINT, &act_int, NULL);
	//
	//SIGNAL ::: Avoid its use: use sigaction(2) instead.  See Portability below.
	//signal(SIGCHLD, signal_handler_child);
	//signal(SIGINT, SIG_IGN); // C^c ne devrait pas marcher
	

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

