#include "main.h"

/**
 * str_append - appends a string to another string
 * @dst: string to append to
 * @src: string to append
 *
 * Return: the new string
 */
char *str_append(char *dst, char *src)
{
	uint dst_len = str_len(dst), src_len = str_len(src);
	char *str = pick(sizeof(char) * (dst_len + src_len + 1));

	memcp(dst, str, dst_len);
	memcp(src, str + dst_len, src_len);
	str[dst_len + src_len] = 0;
	return (str);
}

/**
 * str_eq - checks if 2 strings are equal
 * @s1: a string
 * @s2: a string
 *
 * Return: 1 if s1 and s2 are equal, else 0
 */
char str_eq(char *s1, char *s2)
{
	while (*s1 != '\0' && *s2 != '\0' && (*s1 == *s2))
		s1++, s2++;
	return (*s1 == '\0' && *s2 == '\0');
}

/**
 * str_has_ch - checks if a string contains a character
 * @str: string to query
 * @c: character to look for
 *
 * Return: 1 if `c` is in `str`, else 0
 */
char str_has_ch(char *str, char c)
{
	if (str != NULL)
		for (; *str != '\0'; str++)
			if (*str == c)
				return (1);
	return (0);
}

/**
 * str_fill - creates a string filled with a specified character
 * @c: character to fill with
 * @len: length of the string
 *
 * Return: the string
 */
char *str_fill(char c, uint len)
{
	char *str = mem_fill(c, len + 1);

	str[len] = '\0';
	return (str);
}

/**
 * chr_printable - checks if a character is an ascii printable character
 * @c: character to check
 *
 * Return: 1 if `c` is printable, else 0
 */
char chr_printable(char c)
{
	return (c >= 33 && c <= 126);
}
