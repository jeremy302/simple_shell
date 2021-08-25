#include "main.h"
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

/**
 * join_paths - joins 2 directory paths
 * @s1: the base path
 * @s2: the relative path
 *
 * Return: a string of `s2` appended to `s1`
 */
char *join_paths(char *s1, char *s2)
{
	char *_s1 = str_append(s1, "/");
	char *res = str_append(_s1, s2);

	drop(_s1);
	return (res);
}
