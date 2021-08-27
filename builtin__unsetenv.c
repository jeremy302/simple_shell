#include "main.h"

/**
 * builtin__unsetenv - handles the `unsetenv` command (removes an env variable)
 * @params: command arguments
 *
 * Return: command exit status
 */
int builtin__unsetenv(Invokable params)
{
	char **args = params.args;
	char *env_var = *args;
	Hashtable *env_htbl = glob_g(VAR_ENV);
	var *obj;
	/* check if variable is a valid env-var */
	if (env_var == NULL)
	{
		put_e("unsetenv: Too few arguments.\n");
		return (1);
	}
	obj = htbl_get(env_htbl, env_var, str_len(env_var));
	if (obj != NULL)
		free_var(obj);
	htbl_rm(env_htbl, env_var, str_len(env_var));
	set_env(env_var, NULL);
	return (0);
}
