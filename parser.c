#include "main.h"

/**
 * trim_spcs - skips the preceeding spaces of a string
 * @stream: string to trim
 * @cursor: cursor to increment by the trimmed spaces number
 *
 * Return: pointer to the character after the preceeding spaces
 */
char *trim_spcs(char *stream, uint *cursor)
{
	uint i = 0;

	while (stream[i] == ' ' && stream[i] != '\0')
		i++;
	*cursor += i;
	return (stream + i);
}

/**
 * parse_var - parses a variable name and replaces it with it's value
 * @str: string to parse
 * @cursor: cursor to increment by the number of consumed characters
 *
 * Return: the parsed string
 */
char *parse_var(char *str, uint *cursor)
{
	uint i = 1, vlen = 0;
	char *val = NULL;
	var *var_obj;
	Hashtable *htbl = glob_g(VAR_ENV);

	if (*str != '$')
		return (NULL);
	if (str[i] == '?' || str[i] == '$')
		i++, vlen++;
	else
		while (is_digit(str[i]) || is_letter(str[i]) || str[i] == '_')
			i++, vlen++;
	if (vlen)
	{
		var_obj = (var *)htbl_get(htbl, str + 1, vlen);
		val = str_clone(var_obj == NULL ? "" : var_obj->val);
	}
	else
		val = str_clone("$");
	if (val == NULL)
		val = pick(1);
	*cursor += vlen + 1;
	return (val);
}

/**
 * parse_sq_str - parses a single-quote string
 * @str: string to parse
 * @cursor: cursor to increment by the number of consumed characters
 *
 * Return: the parsed string
 */
char *parse_sq_str(char *str, uint *cursor)
{
	uint i = 0, j = 0, val_size = 20;
	char *val = pick(sizeof(char) * val_size);

	if (str[i] == '\'')
	{
		i++;
	}
	while (str[i] != '\'' && str[i] != '\0')
	{
		enpick(j + 2, val, char, val_size, 20);

		val[j++] = str[i++];
	}
	val[j] = '\0';
	i++;
	*cursor += i;
	return (val);
}

/**
 * parse_dq_str - parses a double-quote string
 * @str: string to parse
 * @cursor: cursor to increment by the number of consumed characters
 *
 * Return: the parsed string
 */
char *parse_dq_str(char *str, uint *cursor)
{
	uint i = 0, j = 0, val_size = 20, var_txt_len;
	char escape = 0, *val = pick(sizeof(char) * val_size), *var_txt;

	if (str[i] == '\"')
		i++;
	while (str[i] != '\0' && (escape || str[i] != '\"'))
	{
		enpick(j + 2, val, char, val_size, 20);
		if ((escape && str[i] == '\"') || (escape && str[i] == '\\') ||
			(escape && str[i] == '$'))
			val[j++] = str[i], i++,	escape = 0;
		else if (escape)
			val[j++] = '\\', val[j++] = str[i],	i++, escape = 0;
		else if (str[i] == '\\')
			escape = 1, i++;
		else if (str[i] == '$')
		{
			var_txt = parse_var(str + i, &i), var_txt_len = str_len(var_txt);
			enpick(j + var_txt_len + 2, val, char, val_size, var_txt_len);
			memcp(var_txt, val + j, var_txt_len), drop(var_txt), j += var_txt_len;
		}
		else
		{
			val[j++] = str[i], i++;
		}
	}
	i++;
	*cursor += i;
	return (val);
}

