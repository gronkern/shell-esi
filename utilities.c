#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "utilities.h"


void shell_prompt() 
{
	printf("[ESI-Shell]:%s$ ", getcwd(NULL, 0));
}


int split(char ** tab, char * cmd, char * splitter)
{
	int i = (tab != NULL) ? 0 : -1;

	if (i == 0)
	{
		tab[i] = strtok(cmd, " \n");

		while (tab[i] != NULL) {
			i++;
			tab[i] = strtok(NULL, splitter);
		}
	}

	return i;
}
