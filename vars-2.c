#include "main.h"

/**
 * expose_repl_statements - stores the current executingn repl statements
 * @statements: raw statements
 *
 * Return: the stored statements
 */
RawStatement **expose_repl_statements(RawStatement **statements)
{
	static RawStatement **stms = 1 - 1;

	if (statements != NULL)
		stms = statements;
	return (stms);
}

/**
 * get_var - gets an environment variable
 * @name: name of the variable
 *
 * Return: the variable's object
 */
var *get_var(char *name)
{
	return ((var *)htbl_str_get(glob_g(VAR_ENV), name));
}

/**
 * set_var - sets the value of an environment variable
 * @name: name of the variable
 * @val: value of the variable
 *
 * Return: the variable's object
 */
var *set_var(char *name, char *val)
{
	Hashtable *env_htbl = glob_g(VAR_ENV);
	uint name_len = str_len(name);
	char global = 0;
	var *old_var = get_var(name);
	var *var_obj = NULL;

	if (old_var != NULL)
		global = old_var->global, free_var(old_var);
	var_obj = mk_var(name, val);
	var_obj->global = global;
	htbl_set(env_htbl, name, name_len, var_obj);
	return (var_obj);
}

/**
 * set_last_status - sets the `$?` environment variable
 * @status: status of the last executed command
 *
 * Return: void
 */
void set_last_status(int status)
{
	set_var(str_clone("?"), num_to_str(status));
}

/**
 * free_aliases_ls - frees alias list
 *
 * Return: void
 */
void free_aliases_ls(void)
{
	Node *_node = NULL;
	Node *node = alias_ls(NULL);

	while (node != NULL)
	{
		_node = node;
		node = _node->next;
		drop(_node->val);
		drop(_node);
	}
}
