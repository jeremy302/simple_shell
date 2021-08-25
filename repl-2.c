#include "main.h"
#include <unistd.h>
#include <limits.h>

/**
 * command_type - determins the command type of a command (e.g builtin or path)
 * @cmd: command
 * @args: arguments
 *
 * Return: the command type
 */
CMD_T command_type(char *cmd, char **args)
{
	BuiltinHandler *handlers = glob_g(VAR_BUILTINS);

	if (str_has_ch(cmd, '/'))
		return (CMD_DIR);
	if (str_eq(cmd, "env") && args[0] != NULL)
		return (CMD_PATH);
	for (; handlers->name != NULL; ++handlers)
		if (str_eq(cmd, handlers->name))
			return (CMD_BUILTIN);
	return (CMD_PATH);
}

/**
 * cmd_no - stores the current command input number
 * @val: current command number
 * @set: whether to set it
 *
 * Return: the current command number
 */
uint cmd_no(uint val, char set)
{
	static uint v = 1 - 1;

	if (set)
		v = val;
	return (v);
}

/**
 * get_cmd_handler - gets the handler for a command
 * @cmd: the command
 * @cmd_type: the command type
 *
 * Return: the command's handler
 */
CommandHandler get_cmd_handler(char *cmd, CMD_T cmd_type)
{
	BuiltinHandler *builtin_handlers = glob_g(VAR_BUILTINS);
	CommandHandler handler;

	switch (cmd_type)
	{
	case CMD_BUILTIN:
		for (; builtin_handlers->name != NULL; ++builtin_handlers)
			if (str_eq(cmd, builtin_handlers->name))
			{
				handler = builtin_handlers->handler;
				break;
			}
		break;
	case CMD_PATH:
		handler = exec_path_cmd;
		break;
	case CMD_DIR:
		handler = exec_dir_cmd;
		break;
	}
	return (handler);
}

/**
 * interprete_statement - interprets a raw statement
 * @statement: a single raw statement
 *
 * Return: the exit status of the execution
 */
int interprete_statement(RawStatement *statement)
{
	int status;
	uint i = 1, args_i = 0, args_size = 5, _i = 0;
	char *cmd = parse_datum(statement->datums[0], &_i);
	char *arg = NULL, **args = pick(sizeof(char *) * args_size);
	CMD_T cmd_type = command_type(cmd, statement->datums + 1);
	CommandHandler handler = NULL;
	Invokable params;


	if (cmd == NULL || *cmd == '\0')
	{
		drop(cmd), drop_2d((void **)args);
		return (0);
	}
	if (cmd_type != CMD_BUILTIN)
	{
		args[0] = cmd_type == CMD_DIR ? str_clone(cmd) : str_clone(cmd);
		++args_i;
	}
	while (statement->datums[i] != NULL)
	{
		arg = parse_datum(statement->datums[i], &_i);
		if (*arg != '\0')
		{
			if (args_i + 1 >= args_size)
			{
				args = repick(args, sizeof(char *) * args_size,
								sizeof(char *) * (args_size + 10));
				args_size += 10;
			}
			args[args_i] = arg, ++args_i;
		}
		++i;
	}
	params.cmd = cmd, params.args = args;
	handler = get_cmd_handler(cmd, cmd_type);
	status = handler(params);
	drop(params.cmd), drop_2d((void **)params.args);
	return (status);
}

/**
 * repl_file - stores the file handle of the repl
 * @handle: a pointer to the repl file handle
 * @force_set: whether to set it
 *
 * Return: a pointer to the handle
 */
int *repl_file(int *handle, char force_set)
{
	static int *_handle = 1 - 1;

	if (handle != NULL || force_set)
		_handle = handle;
	return (_handle);
}

