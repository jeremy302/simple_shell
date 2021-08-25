#include "main.h"

/**
 * str_clone - creates a copy of a string
 * @str: a string
 *
 * Return: the new string
 */
char *str_clone(char *str)
{
	if (str == NULL)
		return (NULL);

	return (mem_clone(str, sizeof(char) * str_len(str) + 1));
}

/**
 * str_chr - returns the pointer to `chr` in `str`
 * @str: the string to query
 * @chr: the character to query
 *
 * Return: a pointer to `chr` in `str`, or a pointer to the end of `str`
 */
char *str_chr(char *str, char chr)
{
	if (str != NULL)
		while (*str != '\0' && *str != chr)
			++str;
	return (str);
}

/**
 * str_split - splits a string into an array of strings
 * @str: string to split
 * @delim: character to split `str` by
 * @max_splits: maximum splits
 *
 * Return: an array of strings
 */
char **str_split(char *str, char delim, int max_splits)
{
	char **splits, capturing = 0, capturing_tmp = 0;
	uint i = 0, splits_size = 2;

	str = str_clone(str);
	splits = pick(sizeof(char *) * splits_size);
	for (; *str != '\0' && (max_splits < 0 || i <= (uint)max_splits); str++)
	{
		capturing_tmp = capturing;
		capturing = *str != delim;
		if (capturing != capturing_tmp)
		{
			if (capturing)
			{
				enpick(i + 2, splits, char *, splits_size, 10);
				splits[i] = str;
				i++;
			}
			else
				*str = '\0';
		}
	}
	return (splits);
}

/**
 * str_ends_with - determines if a string ends with another string
 * @str: string to query
 * @sub_str: string to search for
 *
 * Return: 1 if `str` ends with `sub_str`
 */
char str_ends_with(char *str, char *sub_str)
{
	size_t str_l = str_len(str), sub_str_l = str_len(sub_str);

	return (memeq(str + (str_l - sub_str_l), sub_str, sub_str_l + 1));
}

/**
 * str_starts_with - determines if a string starts with another string
 * @str: string to query
 * @sub_str: string to search for
 *
 * Return: 1 if `str` starts with `sub_str`
 */
char str_starts_with(char *str, char *sub_str)
{
	size_t sub_str_l = str_len(sub_str);

	return (memeq(str, sub_str, sub_str_l));
}

