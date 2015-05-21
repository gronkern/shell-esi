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
		tab[i] = strtok(cmd, splitter);

		while (tab[i] != NULL) {
			i++;
			tab[i] = strtok(NULL, splitter);
		}
	}

	return i;
}

int find_first(const char ** tokens, unsigned size, const char * token)
{
	unsigned i = 0;

	while (i < size && tokens[i] != NULL && strcmp(tokens[i], token) != 0)
		i++;

	return (i < size && tokens[i] != NULL &&  strcmp(tokens[i], token) == 0) 
		? i 
		: -1;
}

int shift_one(char ** tokens, unsigned pos, unsigned size, unsigned shift)
{
	int shifted_one = (tokens == NULL || pos >= size || size - shift - 1 < 0) 
		? 0 
		: 1;

	if (shifted_one == 1)
		for (int i = size - 1; i >= pos + shift; i--)
			tokens[i] = tokens[i - shift];
	
	return shifted_one;
}
