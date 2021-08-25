#include "main.h"

/**
 * parse_datum - parses a datum string
 * @str: string to parse
 * @cursor: cursor to increment by the number of consumed characters
 *
 * Return: the parsed string
 */
char *parse_datum(char *str, uint *cursor)
{
	uint i = 0, j = 0, val_size = 20, exp_len = 0;
	char esc = 0, *val = pick(sizeof(char) * 20), *exp = NULL;

	if (str == NULL)
		return (val);
	while (str[i] == ' ')
		i++;
	if (str[i] == '#')
		goto exit;
	while (esc || !(str[i] == ' ' || str[i] == '&' || str[i] == '|' ||
					str[i] == ';' || str[i] == '\n' || str[i] == '\0'))
	{
		enpick(j + 2, val, char, val_size, 20);
		if (esc)
			val[j++] = str[i++], esc = 0;
		else if (str[i] == '\\')
			esc = 1, i++;
		else if (str[i] == '\'')
		{
			exp = parse_sq_str(str + i, &i), exp_len = str_len(exp);
			enpick(j + exp_len + 2, val, char, val_size, exp_len);
			memcp(exp, val + j, exp_len), drop(exp), j += exp_len;
		}
		else if (str[i] == '\"')
		{
			exp = parse_dq_str(str + i, &i), exp_len = str_len(exp);
			enpick(j + exp_len + 2, val, char, val_size, exp_len);
			memcp(exp, val + j, exp_len), drop(exp), j += exp_len;
		}
		else if (str[i] == '$')
		{
			exp = parse_var(str + i, &i),	exp_len = str_len(exp);
			enpick(j + exp_len + 2, val, char, val_size, exp_len);
			memcp(exp, val + j, exp_len), drop(exp), j += exp_len;
		}
		else
			val[j++] = str[i++];
	}
exit:
	return (*cursor += i, val);
}

/**
 * parse_delimiter - parses a delimiter (an statement linker/delimiter)
 * @str: string to parse
 * @cursor: cursor to increment by the number of consumed characters
 *
 * Return: the delimiter enum
 */
DELIMITER parse_delimiter(char *str, uint *cursor)
{
	uint i = 0;

	while (*str == ' ')
		i++, str++;
	if (*str == ';')
	{
		*cursor += i + 1;
		return (DELIM_STATEMENT);
	}
	else if (str_starts_with(str, "||"))
	{
		*cursor += i + 2;
		return (DELIM_OR);
	}
	else if (str_starts_with(str, "&&"))
	{
		*cursor += i + 2;
		return (DELIM_AND);
	}
	else if (*str == '#')
	{
		*cursor += i + 1;
		return (DELIM_END);
	}
	else if (*str == '\n')
	{
		*cursor += i + 1;
		return (DELIM_END);
	}
	else if (*str == '\0')
	{
		*cursor += i;
		return (DELIM_END);
	}
	else
	{
		*cursor += i;
		return (DELIM_NONE);
	}
}

/**
 * parse_raw_datums - parses a multi-datum string, but doesn't expand it
 * @str: string to parse
 * @cursor: cursor to increment by the number of consumed characters
 *
 * Return: an array of the datum strings
 */
char **parse_raw_datums(char *str, uint *cursor)
{
	uint i = 0, size = 1, datum_i = 0;
	char *datum = NULL, **datums = pick(sizeof(char *) * size), repeat = 0;

	do {
		i = 0;
		datum = parse_datum(str + i, &i);
		if (*datum != '\0')
		{
			enpick(datum_i + 1, datums, char *, size, 5);
			datums[datum_i] = pick(sizeof(char) * i + 1);
			memcp(str, datums[datum_i], i);
			++datum_i;
		}
		str += i;
		*cursor += i;
		repeat = *datum != '\0';
		drop(datum);
	} while (repeat);
	return (datums);
}

