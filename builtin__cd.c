#include "main.h"
#include <unistd.h>
#include <limits.h>

/**
 * builtin__cd - handles the `cd` command (changes the working directory)
 * @params: command arguments
 *
 * Return: command exit status
 */
int builtin__cd(Invokable params)
{
	char **args = params.args;
	char cwd[PATH_MAX];
	char old_wd[PATH_MAX];
	char *dir = *args, is_old = 0;
	var *old_pwd = NULL;
	var *home = NULL;
	/* Hashtable *env_htbl = glob_g(VAR_ENV); */

	getcwd(old_wd, sizeof(old_wd));
	if (dir == NULL)
		home = get_var("HOME"), dir = home == NULL ? NULL : home->val;
	else if (str_eq(dir, "-"))
	{
		is_old = 1, old_pwd = get_var("OLDPWD");
		dir = old_pwd == NULL ? NULL : old_pwd->val;
		if (dir == NULL)
			puts(old_wd);
	}
	if (dir == NULL)
		return (0); /* use correct status later */
	if (chdir(dir) == 0)
	{
		getcwd(cwd, sizeof(cwd)); /* check that size if enough */
		if (is_old)
			put_s(cwd), put_s("\n");
		set_var(str_clone("PWD"), str_clone(cwd));
		set_var(str_clone("OLDPWD"), str_clone(old_wd));
		return (0);
	}
	else
	{
		put_ef("cd: can't cd to "), put_e(dir),	put_e("\n");
		return (2);
	}
	return (0);
}
