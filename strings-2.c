#include "main.h"

/**
 * str_len - gets the length of a string
 * @str: a string
 *
 * Return: the length of `str`
 */
size_t str_len(char *str)
{
	size_t len = 0;

	if (str != NULL)
		while (*str++ != '\0')
			++len;
	return (len);
}

