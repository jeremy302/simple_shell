#include "main.h"

/**
 * env_init - loads environment variables from the programs env argument
 * @renv: env reference
 * @renv_len: env_len reference
 * @renv_size: env_size reference
 *
 * Return: void
 */
void env_init(char ***renv, uint *renv_len, uint *renv_size)
{
	uint i, env_len = 0, env_size = *renv_size;
	char **env = *renv, **prog_env = get_env(NULL, 0);

	while (prog_env[env_len] != NULL)
		env_len++;
	env_size = env_len + 21;
	env = pick(sizeof(char *) * (env_size));
	for (i = 0; i < env_len; ++i)
		env[i] = str_clone(prog_env[i]);

	*renv = env, *renv_len = env_len, *renv_size = env_size;
}

/**
 * env_set - sets an environment variable at a location in env
 * @env: environment variables array
 * @index: index to set
 * @name: variable name
 * @value: variable value
 *
 * Return: void
 */
void env_set(char **env, uint index, char *name, char *value)
{
	char *v;
	uint name_len = str_len(name), val_len = str_len(value);

	v = pick(sizeof(char) * (name_len + val_len + 2));
	memcp(name, v, name_len);
	v[name_len] = '=';
	memcp(value, v + name_len + 1, val_len);
	env[index] = v;
}

/**
 * set_env - adds/updates/deletes an environment variable
 * @name: variable name. use NULL to just get env
 * @value: variable value. use NULL to remove value
 *
 * Return: env
 */
char **set_env(char *name, char *value)
{
	static char initted = 1 - 1, **env = 1 - 1;
	static uint env_size = 1 - 1, env_len = 1 - 1;
	uint i = 0;
	char *key = name, *val = value, found = 0, *v, *eq_v;

	if (!initted)
		initted = 1, env_init(&env, &env_len, &env_size);
	if (name == NULL)
		return (env);
	for (i = 0; i < env_len; ++i)
	{
		v = env[i], eq_v = str_chr(v, '=');
		if (memeq(v, name, eq_v - v))
		{
			found = 1, drop(v);
			if (val == NULL)
			{
				--env_len;
				do
					++i, env[i - 1] = env[i];
				while (env[i] != NULL);
			}
			else
				env_set(env, i, key, val);
			break;
		}
	}
	if (!found && val != NULL)
	{
		enpick(env_len + 2, env, char *, env_size, 20);
		env_set(env, env_len, key, val), env_len++;
	}
	return (env);
}

/**
 * free_env - frees all environment variables
 *
 * Return: void
 */
void free_env(void)
{
	char **env = set_env(NULL, NULL);

	drop_2d((void **)env);
}
