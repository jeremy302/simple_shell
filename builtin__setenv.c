#include "main.h"

/**
 * builtin__setenv - handles the `setenv` command
 * @params: command arguments
 *
 * Return: command exit status
 */
int builtin__setenv(Invokable params)
{
	char **args = params.args;
	char *var_name = args[0];
	char *var_val = var_name == NULL ? NULL : args[1];
	var *obj = NULL;

	/* TODO: validate var_name */
	if (args[0] == NULL)
	{
		/* builtin__env(params); */
		return (0);
	}
	else if (args[1] != NULL && args[2] != NULL)
	{
		/* TODO: should input number be printed? */
		put_ef("Too many arguments.\n");
		return (1);
	}
	else if (args[1] != NULL)
	{
		obj = set_var(str_clone(var_name), str_clone(var_val));
		set_env(var_name, var_val);
		obj->global = 1;
	}
	return (0);
}
