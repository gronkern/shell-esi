#include <string.h>

#include "utilities.h"

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
