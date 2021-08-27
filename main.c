#include "main.h"
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <fcntl.h>

/**
 * quit - exits the program after releasing resources and saving history
 * @status: exit status of the program
 *
 * Return: void
 */
void quit(int status)
{
	char status_success;

	if (status == INT_MIN)
		status = parse_exit_status(get_var("?")->val, &status_success);
	save_history();
	free_globals();
	exit(status);
}

/**
 * prog_args - stores the shell's arguments
 * @args: arguments
 * @set: whether to set the arguments
 *
 * Return: the arguments
 */
char **prog_args(char **args, char set)
{
	static char **v = 1 - 1;

	if (set)
		v = args;
	return (v);
}

/**
 * init - Prepares the program by loading `$PS1` and the history
 * @args: shell's arguments
 *
 * Return: void
 */
void init(char **args)
{
	/* Hashtable *env_htbl = glob_g(VAR_ENV); */
	var *PS1 = get_var("PS1");
	var *HOME = get_var("HOME");
	var *PATH = get_var("PATH");

	prog_args(args, 1);
	set_last_status(0);

	if (PATH == NULL)
		set_var(str_clone("PATH"), str_clone(""));
	if (PS1 == NULL)
	{
		PS1 = set_var(str_clone("PS1"), str_clone("($) "));
		PS1->global = 0;
	}
	if (NULL != HOME && NULL != HOME->val)
		set_var(str_clone("HISTFILE"),
				join_paths(HOME->val, ".simple_shell_history"));
	else
		set_var(str_clone("HISTFILE"), str_clone(""));
	set_var(str_clone("$"), num_to_str(getpid()));
	load_history();

	signal(SIGINT, sigint_handler);
}

/**
 * get_env - stores environment variables
 * @env: environment variables
 * @set: whther to set _env
 *
 * Return: the stored environment varaibles
 */
char **get_env(char **env, char set)
{
	static char **_env = 1 - 1;

	if (set)
		_env = env;
	return (_env);
}

/**
 * main - starts the shell
 * @argc: arguments count
 * @argv: arguments
 * @env: environment variables
 *
 * Return: the last exit status
 */
int main(int argc, char **argv, char **env)
{
	int handle;

	get_env(env, 1);
	init(argv);
	if (argc > 1)
	{
		handle = open(argv[1], O_RDONLY);
		if (handle == -1)
			put_ef("Can't open "), put_e(argv[1]), put_e("\n"), quit(127);
		else
			start_repl(handle);
	}
	else
	{
		start_repl(STDIN_FILENO);
	}
	quit(0);
	return (0);
}
