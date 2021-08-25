#include "main.h"

/**
 * print_alias_expansion - prints the alias expansion
 * @str: alias expansion
 *
 * Return: void
 */
static void print_alias_expansion(char *str)
{
	char tmp[] = "0";
	char prev_chr = '\0';

	put_s("'");
	for (; *str != '\0'; ++str)
	{
		prev_chr = *str;
		if (*str == '\'')
		{
			put_s("'\"'\"");
			if (str[1] != '\0')
				put_s("'");
		}
		else
		{
			*tmp = *str, put_s(tmp);
		}
	}
	if (prev_chr != '\'')
		put_s("'");
}

/**
 * print_aliases - prints all aliases
 * @aliases_htbl: aliases hashtable
 *
 * Return: void
 */
void print_aliases(Hashtable *aliases_htbl)
{
	HashItem *alias_hash_obj = NULL;
	Node *alias_node = NULL;

	alias_node = htbl_node_iter(aliases_htbl, alias_node);
	for (; alias_node != NULL;
		 alias_node = htbl_node_iter(aliases_htbl, alias_node))
	{
		alias_hash_obj = (HashItem *)alias_node->val;
		put_s(alias_hash_obj->key);
		put_s("=");
		print_alias_expansion(alias_hash_obj->val);
		put_s("\n");
	}
}

/**
 * assign_aliases - associates expansion strings with aliase names
 * @raliases_htbl: aliases hashtable reference
 * @rarg: arg variable reference
 * @ralias_splits: alias_splits reference
 * @ralias_exp: alias_exp reference
 * @ralias_name: alias_name reference
 *
 * Return: void
 */
void assign_aliases(Hashtable **raliases_htbl, char **rarg,
				   char ***ralias_splits, char **ralias_exp, char **ralias_name)
{
	Hashtable *aliases_htbl = *raliases_htbl;
	char *arg = *rarg, **alias_splits = *ralias_splits,
		*alias_exp = *ralias_exp, *alias_name = *ralias_name;

	alias_splits = str_split(arg, '=', 1), alias_name = alias_splits[0];
	alias_exp = htbl_str_get(aliases_htbl, arg);
	if (alias_exp != NULL)
		drop(alias_exp);
	alias_exp = str_clone(alias_splits[1]);
	htbl_set(aliases_htbl, alias_name, str_len(alias_name), alias_exp);
	drop(alias_splits[0]);
	drop(alias_splits);

	*raliases_htbl = aliases_htbl, *rarg = arg, *ralias_splits = alias_splits,
		*ralias_exp = alias_exp, *ralias_name = alias_name;
}

/**
 * builtin__alias - handles the `alias` command
 * @params: command arguments
 *
 * Return: command exit status
 */
int builtin__alias(Invokable params)
{
	Hashtable *aliases_htbl = glob_g(VAR_ALIASES);
	int status = 0, args_i = 0, args_queue_i = 0, exps_queue_i = 0,
		args_queue_size = 10, exps_queue_size = 10, print_i = 0;
	char **args = params.args, *arg, **alias_splits, *alias_name = NULL,
		*alias_exp = NULL, **args_queue = pick(sizeof(char *) * args_queue_size),
		**exps_queue = pick(sizeof(char *) * exps_queue_size);

	if (args[0] == NULL)
		print_aliases(aliases_htbl);
	else
	{
		for (args_i = 0; args[args_i] != NULL; ++args_i)
		{
			arg = args[args_i];
			if (str_has_ch(arg, '='))
				assign_aliases(&aliases_htbl, &arg, &alias_splits,
							   &alias_exp, &alias_name);
			else
			{
				alias_exp = htbl_str_get(aliases_htbl, arg);
				if (alias_exp == NULL)
					status = 1,	put_e("alias: "), put_e(args[args_i]), put_e(" not found\n");
				else
				{
					enpick(args_queue_i + 2, args_queue, char *, args_queue_size, 10);
					enpick(exps_queue_i + 2, exps_queue, char *, exps_queue_size, 10);
					args_queue[args_queue_i++] = str_clone(arg),
					exps_queue[exps_queue_i++] = str_clone(alias_exp);
				}
			}
		}
		for (print_i = 0; print_i < args_queue_i; ++print_i)
			put_s(args_queue[print_i]),	put_s("="),
				print_alias_expansion(exps_queue[print_i]), put_s("\n");
	}
	drop_2d((void **)args_queue), drop_2d((void **)exps_queue);
	return (status);
}