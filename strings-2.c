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

/**
 * str_contains - determines if a string has another string
 * @str: string to query
 * @sub_str: string to search for
 *
 * Return: 1 if `str` has `sub_str`
 */
char str_contains(char *str, char *sub_str)
{
	char *str_tmp, *sub_str_tmp = sub_str;

	if (str != NULL && sub_str != NULL)
		while (*str != '\0')
		{
			str_tmp = str;
			while (*str != '\0' && *sub_str != '\0' && *str == *sub_str)
				++str, ++sub_str;
			if (*sub_str == '\0')
				return (1);
			sub_str = sub_str_tmp, str = str_tmp + 1;
		}
	return (0);
}

