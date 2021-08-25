#include "main.h"
#include <unistd.h>

/**
 * _unum_to_str - converts an unsinged long to a string
 * @num: number to converts
 * @str: string to write to
 *
 * Return: the next string to write the next digit (if any) to
 */
static char *_unum_to_str(ulong num, char *str)
{
	ulong div = num / 10;
	char rem = num % 10;

	if (div == 0)
		*str = rem + '0', str[1] = '\0';
	else
		str = _unum_to_str(div, str), *str = rem + '0', str[1] = '\0';
	return (str + 1);
}

/**
 * num_to_str - converts a long to a string
 * @num: number to convert
 *
 * Return: `num`'s string representation
 */
char *num_to_str(long num)
{
	ulong v;
	char *str = pick(sizeof(char) * 25);

	v = num;
	if (num < 0)
	{
		*str++ = '-';
		v = -num;
	}
	_unum_to_str(v, str);
	return (str);
}

/**
 * unum_to_str - converts an unsigned long to a string
 * @num: number to convert
 *
 * Return: `num`'s string representation
 */
char *unum_to_str(ulong num)
{
	char *str = pick(sizeof(char) * 25);

	_unum_to_str(num, str);
	return (str);
}
