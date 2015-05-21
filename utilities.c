#include "utilities.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


/*
 * getcwd : nom du dossier courant
 *
 * retourne : 
 *  - une chaine \0 terminée sans erreur
 *  - NULL en cas d'erreur
 *
 * La chaine du directory doit être liberée
 *
 */
void shell_prompt() 
{
	char * directory = NULL;

	if ((directory = getcwd(NULL, 0)) != NULL)
	{
		printf("[ESI-Shell]:%s$ ", directory);
		free(directory);
	}
}

void shell_exit(const char * message, int status)
{
	printf("%s \n", (message == NULL) ? "" : message);
	exit(status);
}

void shell_exit_error(const char * message)
{
	perror((message == NULL) ? "" : message);
	exit(EXIT_FAILURE);
}

/**
 * Une allocation interne est plus appropriée, peut être que l'utilisateur n'a
 * pas alloué assez de place. A méditer.
 */
int split(char ** tab, char * cmd, char * splitter)
{
	int i = (tab != NULL && cmd != NULL && splitter != NULL) ? 0 : -1;

	if (i == 0)
	{
		tab[i] = strtok(cmd, "\t \n");

		while (tab[i] != NULL) {
			i++;
			tab[i] = strtok(NULL, splitter);
		}
	}

	return i;
}

char * find_first(const char ** tokens, const char * token)
{
	char * find_token = NULL;
	int i = 0;

	while (tokens[i] != NULL && strcmp(tokens[i], token))
		i++;

	if (tokens[i] != NULL)
		find_token = (char *) tokens[i];

	return find_token;
}

int shift_one(char ** tokens, unsigned pos, unsigned size)
{
	int shifted_one = (tokens == NULL || pos >= size) ? -1 : 1;

	if (shifted_one == 1)
	{
		for (int i = size - 2; i >= pos; i--)
			tokens[i + 1] = tokens[i];
	}
	
	return shifted_one;
}
