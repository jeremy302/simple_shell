#include "main.h"
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

/**
 * current_exec - stores the current child process executing the command
 * @pid: child process pid
 * @force_set: whether to set the pid
 *
 * Return: the current child process pid
 */
pid_t *current_exec(pid_t *pid, char force_set)
{
	static pid_t *_pid = 1 - 1;

	if (pid != NULL || force_set)
		_pid = pid;
	return (_pid);
}

/**
 * exec - executes a program with the specified arguments
 * @pathname: path to the program to execute
 * @args: arguments to pass to the program
 *
 * Return: the exit code of the program
 */
int exec(char *pathname, char *args[])
{
	pid_t pid = fork();
	int status = 0;

	/* TODO handle failure with getpid */
	if (pid)
	{
		current_exec(&pid, 1);
		waitpid(pid, &status, 0);
		current_exec(NULL, 1);
	}
	else
	{
		execve(pathname, args, set_env(NULL, NULL));
		put_ef("Couldn't create child process\n");
		return (126);
	}
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else
		status = 0;
	return (status);
}


/**
 * search_path - searches the directories in the `$PATH` variable for a program
 * @str: program to search for
 *
 * Return: the path of the program if it was found, else NULL
 */
char *search_path(char *str)
{
	uint paths_i = 0;
	char *path = ((var *) htbl_str_get(glob_g(VAR_ENV), "PATH"))->val;
	char **paths = NULL, new_path = 0;

	if (str_starts_with(path, ":") || str_ends_with(path, ":") ||
		str_contains(path, "::"))
		new_path = 1, path = str_append(path, ":./");
	paths = str_split(path, ':', -1);
	if (new_path)
		drop(path);
	for (; paths[paths_i] != NULL; ++paths_i)
	{
		char *filepath = join_paths(paths[paths_i], str);

		if (access(filepath, F_OK) == 0)
		{
			drop(paths[0]), drop(paths);
			return (filepath);
		}
		drop(filepath);
	}
	drop(paths[0]), drop(paths);
	return (NULL);
}

/**
 * exec_path_cmd - handles shell commands whose commands were found in `$PATH`
 * @params: command to execute, and it's arguments
 *
 * Return: the exit status of the program
 */
int exec_path_cmd(Invokable params)
{
	char *filepath = search_path(params.cmd);
	int status;

	if (filepath == NULL)
	{
		put_ef(params.cmd);
		put_e(": not found\n");
		return (127);
	}
	if (!(access(filepath, R_OK) == 0 && access(filepath, X_OK) == 0))
	{
		put_ef(params.cmd);
		put_e(": Permission denied\n");
		drop(filepath);
		return (126);
	}
	status = exec(filepath, params.args);
	drop(filepath);
	return (status);
}

/**
 * exec_dir_cmd - handles shell commands whose commands are paths to a file
 * @params: command to execute, and it's arguments
 *
 * Return: the exit status of the program
 */
int exec_dir_cmd(Invokable params)
{
	char *filepath = params.cmd;
	int status;

	if (filepath == NULL)
	{
		put_ef(params.cmd);
		put_e(": not found\n");
		return (127);
	}
	if (!(access(filepath, R_OK) == 0 && access(filepath, X_OK) == 0))
	{
		put_ef(params.cmd);
		put_e(": Permission denied\n");
		return (126);
	}
	status = exec(filepath, params.args);
	return (status);
}
