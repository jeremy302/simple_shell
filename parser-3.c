#include "main.h"

/**
 * parse_raw_statement - parses statements without expanding them
 * @str: string to parse
 *
 * Return: an array of the parsed `RawStatement`s
 */
RawStatement **parse_raw_statements(char *str)
{
	uint i = 0, size = 1, stm_i = 0;
	RawStatement *stm, **statements = pick(sizeof(RawStatement *) * size);

	do {
		i = 0;

		stm = pick(sizeof(RawStatement));
		stm->datums = parse_raw_datums(str + i, &i);
		stm->delim = parse_delimiter(str + i, &i);

		enpick(stm_i + 1, statements, RawStatement *, size, 5);
		statements[stm_i] = stm;
		++stm_i;
		str += i;
	} while (stm->delim != DELIM_END);
	return (statements);
}

/**
 * free_raw_statements - frees a rawstatement array
 * @statements: statements to free
 *
 * Return: void
 */
void free_raw_statements(RawStatement **statements)
{
	uint i = 0;

	if (statements == NULL)
		return;
	for (i = 0; statements[i] != NULL; ++i)
	{
		if (statements[i]->datums != NULL)
			drop_2d((void **)statements[i]->datums);
	}
	drop_2d((void **)statements);
}

/**
 * is_alias_name - checks if a string is a valid alias name
 * @str: string to check
 *
 * Return: 1 if `str` is a valid alias name, else 0
 */
char is_alias_name(char *str)
{
	uint i = 0;

	for (; str[i] != '\0'; ++i)
	{
		if (i == 0 && str[i] == '=')
			continue;
		if (str_has_ch(" \t\n/$`=|&;()<>'\"\\", str[i]) ||
			!chr_printable(str[i]))
			return (0);
	}
	return (i != 0);
}

/**
 * delim_to_str - converts a `DELIMITER` enum value to a string
 * @delim: a `DELIMITER` enum value
 *
 * Return: a string
 */
char *delim_to_str(DELIMITER delim)
{
	char *str = NULL;

	switch (delim)
	{
	case DELIM_STATEMENT:
		str = str_clone(";");
		break;
	case DELIM_OR:
		str = str_clone("||");
		break;
	case DELIM_AND:
		str = str_clone("&&");
		break;
	case DELIM_END:
		str = str_clone("\0");
		break;
	case DELIM_NONE:
		str = str_clone("\n");
	default:
		break;
	}
	return (str);
}

/**
 * expand_aliases - expands aliases in a `RawStatement` array
 * @stms: statements to expand
 * @exped_htbl: hashtable of aliases that have already been expanded
 *
 * Return: a string of the expanded statements
 */
char *expand_aliases(RawStatement **stms, Hashtable *exped_htbl)
{
	uint stm_i, datum_i, datum_blanks = 0;
	Hashtable *aliases_htbl = glob_g(VAR_ALIASES);
	char *datum, *exp, *result = str_clone(""), *_result, c_exp = 1,
		release_exp_htbl = exped_htbl == NULL, *_exp, *delim_str, *blanks;
	RawStatement **exp_stms;

	exped_htbl = exped_htbl == NULL ? make_htbl(50) : exped_htbl;
	for (stm_i = 0; *(stms + stm_i) != NULL; ++stm_i)
	{
		for (c_exp = 1, datum_i = 0; stms[stm_i]->datums[datum_i] != NULL; ++datum_i)
		{
			datum = stms[stm_i]->datums[datum_i];
			while (datum[datum_blanks] == ' ')
				++datum_blanks;
			exp = c_exp && is_alias_name(datum + datum_blanks)
				&& htbl_str_get(exped_htbl, datum + datum_blanks) == NULL ?
				str_clone(htbl_str_get(aliases_htbl, datum + datum_blanks)) : NULL;
			if (exp != NULL)
			{
				exp_stms = NULL, blanks = str_fill(' ', datum_blanks);
				_exp = exp,	exp = str_append(blanks, exp), drop(blanks), drop(_exp);
				htbl_set(exped_htbl, datum, str_len(datum), (void *)1);
				exp_stms = parse_raw_statements(exp);
				_exp = expand_aliases(exp_stms, exped_htbl);
				_result = result, result = str_append(result, _exp);
				drop(_result), drop(_exp), free_raw_statements(exp_stms);
				if (!(str_ends_with(exp, " ") || str_ends_with(exp, "\t") ||
					  str_ends_with(exp, "\n")))
					c_exp = 0;
				drop(exp);
			}
			else
				_result = result, result = str_append(result, datum),
					drop(_result), c_exp = 0;
		}
		_result = result, delim_str = delim_to_str(stms[stm_i]->delim);
		result = str_append(result, delim_str), drop(_result), drop(delim_str);
	}
	release_exp_htbl ? free_htbl(exped_htbl, NULL) : (void)0;
	return (result);
}
