#ifndef UTILITIES_H
#define UTILITIES_H

/**
 * Split une chaine en un tableau de chaine selon le délimiteur choisi
 * \param tokens Le tableau à remplir, il sera NULL terminé.
 * \param command La chaine à splitter.
 * \param splitter La chaine reprenant les délimiteurs.
 *
 * \return L'indice de l'élément NULL, soit le nombre LOGIQUE d'éléments du
 * tableau ou -1 en cas d'erreur. [0 - n - 1] + NULL OU -1 en cas d'erreur.
 */
int split(char ** tokens, char * command, char * splitter);

/**
 * Affiche le prompt
 */
void shell_prompt();

/**
 *
 */
void shell_exit(const char * message, int status);

/**
 *
 */
void shell_exit_error(const char * message);

char * find_first(const char ** tokens, const char * token);

int shift_one(char ** tokens, unsigned pos, unsigned size);
#endif
