#include "main.h"

/**
 * mk_var - creates an environment variable object
 * @name: variable's name
 * @val: variable's value
 *
 * Return: the created variable
 */
var *mk_var(char *name, char *val)
{
	/* uint name_size = str_len(name) + 1, val_size = str_len(val) + 1; */
	void *block = pick(sizeof(var));
	var *obj = block;

	obj->global = 0;
	obj->name = name;/* (char *)block + sizeof(var); */
	obj->val = val;/* (char *)block + sizeof(var) + name_size; */
	return (obj);
}

/**
 * free_var - frees a variable
 * @obj: variable to free
 *
 * Return: void
 */

void free_var(var *obj)
{
	if (obj != NULL)
		drop(obj->name), drop(obj->val);
	drop(obj);
}

/**
 * reg_env - parses the initial enviroment variables of the program
 *
 * Return: 1 if successful, else 0
 */
char reg_env(void)
{
	char **env = get_env(NULL, 0);
	uint i = 0;
	Hashtable *htbl = glob_g(VAR_ENV);

	for (; env[i] != NULL; ++i)
	{
		char *str = env[i];
		char c_tmp;
		uint len = str_len(str);
		var *obj =  pick(sizeof(var));
		char *eq_ptr = str_chr(str, '=');
		uint eq_i = eq_ptr - str;

		c_tmp = *eq_ptr;
		*eq_ptr = '\0';
		obj->global = 1;
		obj->name = str_clone(str);
		obj->val = eq_i == len ? str_clone("") : str_clone(eq_ptr + 1);
		htbl_set(htbl, obj->name, eq_i, obj);
		*eq_ptr = c_tmp;
	}
	return (1);
}

/**
 * glob_g - gets a global variable that is stored statically in the function
 * @type: type of the variable to return
 *
 * Return: the requested variable
 */
void *glob_g(Glob_t type)
{
	static Hashtable *ENV = 1 - 1;
	static Hashtable *ALIASES =  1 - 1;
	static char CHAR_ALLOCS[htbl_sizeof(1000)] = {0};
	static Hashtable *ALLOCS = (Hashtable *)CHAR_ALLOCS;

	static BuiltinHandler BUILTINS[] = {
		{"cd", &builtin__cd},
		{"env", &builtin__env},
		{"setenv", &builtin__setenv},
		{"unsetenv", &builtin__unsetenv},
		{"alias", &builtin__alias},
		{"history", &builtin__history},
		{"help", &builtin__help},
		{"exit", &builtin__exit},
		{NULL, NULL}
	};

	switch (type)
	{
	case VAR_ENV:
		if (ENV == NULL)
			ENV = make_htbl(1000), reg_env();
		return ((void)-1, ENV);
	case VAR_ALIASES:
		if (ALIASES == NULL)
			ALIASES = make_htbl(1000);
		return (ALIASES);
	case VAR_ALLOCS:
		if (ALLOCS->size == 0)
		{
			init_htbl(ALLOCS, 1000);
		}
		return (ALLOCS);
	case VAR_BUILTINS:
		return (&BUILTINS);
	default:
		return (NULL);
	}
}


/**
 * free_globals - frees all "global" memory
 *
 * Return: void
 */
void free_globals(void)
{
	Hashtable *env_htbl = glob_g(VAR_ENV);
	Hashtable *aliases_htbl = glob_g(VAR_ALIASES);
	Hashtable *allocs_htbl = glob_g(VAR_ALLOCS);

	free_htbl(env_htbl, (val_freer)(&free_var));
	free_htbl(aliases_htbl, dropv);

	free_history();
	free_raw_statements(expose_repl_statements(NULL));

	free_env();

	drop_all();
	(void) allocs_htbl, (void) aliases_htbl, (void) env_htbl;
	/* free_static_htbl(allocs_htbl, NULL); */
}

