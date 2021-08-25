#include "main.h"

/**
 * parse_exit_status - parses a string exit status
 * @str: exit status
 * @success: char to set to 1 if the exit status was parsed correctly
 *
 * Return: the exit status
 */
int parse_exit_status(char *str, char *success)
{
	ulong num = 0;

	*success = 1;
	if (*str == '+')
		str++;
	if (*str == '\0')
	{
		*success = 0;
		return (2);
	}
	for (; *str != '\0'; str++)
	{
		if (!is_digit(*str))
		{
			*success = 0;
			return (2);
		}
		num = (num * 10) + (*str - '0');
		if (num > 2147483647)
		{
			*success = 0;
			return (2);
		}
	}
	return (num);
}

/**
 * builtin__exit - handles the `exit` command (exits the program)
 * @params: command arguments
 *
 * Return: command exit status
 */
int builtin__exit(Invokable params)
{
	char **args = params.args;
	char *status_str = *args, valid_status = 0;
	var *status_obj = get_var("?");
	int status;

	if (status_str == NULL)
		status_str = status_obj == NULL ?
			str_clone("0") : str_clone(status_obj->val);

	status = parse_exit_status(status_str, &valid_status);
	if (status_str != *args)
		drop(status_str);
	if (!valid_status)
	{
		put_ef("exit: Illegal number: ");
		put_e(status_str);
		put_e("\n");
	}
	else
	{
		drop(params.cmd);
		drop_2d((void **)params.args);
		quit(status % 256);
	}
	return (status % 256);
}
