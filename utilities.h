/**
 * Split une chaine en un tableau de chaine selon le délimiteur choisi
 * \param tab Le tableau à remplir, il sera NULL terminé.
 * \param cmd La chaine à splitter.
 * \param splitter La chaine reprenant les délimiteurs.
 *
 * \return L'indice de l'élément NULL, soit le nombre LOGIQUE d'éléments du
 * tableau ou -1 en cas d'erreur.
 */
int split(char **, char *, char *);

/**
 * Affiche le prompt
 */
void shell_prompt();
