#include "main.h"
#include <fcntl.h>
#include <unistd.h>

#define HISTORY_STACK_SIZE 4096

/**
 * hist_pstack - adds an item to the history stack (and remove older items)
 * @ln: item to add
 * @is_old: whether the item is being loaded from an old history file
 * @get_old: if a pointer starting from the oldest item is returned
 *
 * Return: the history stack
 */
char **hist_pstack(char *ln, char is_old, char get_old)
{
	static uint history_i = 1 - 1, old_i =  1 - 1;
	static char *history[HISTORY_STACK_SIZE + 1] = {NULL};
	uint i = 0;

	if (ln != NULL)
	{
		if (history_i >= HISTORY_STACK_SIZE)
		{
			drop(history[0]);
			for (i = 0; i + 1 < HISTORY_STACK_SIZE; ++i)
			{
				history[i] = history[i + 1];
			}
			history[i] = ln;
			if (!is_old)
				old_i--;
		}
		else
		{
			history[history_i++] = ln;
			if (is_old)
				old_i++;
		}
	}
	return (get_old ? history : history + old_i);
}

/**
 * free_history - frees the history stack
 *
 * Return: void
 */
void free_history(void)
{
	uint history_i = 0;
	char **history = hist_pstack(NULL, 0, 1);

	for (; history[history_i] != NULL; ++history_i)
		drop(history[history_i]);
}
/**
 * load_history - Initializes the history stack from the history file
 *
 * Return: the history stack
 */
char **load_history()
{
	static char loaded = 1 - 1;

	if (loaded)
		return (hist_pstack(NULL, 1, 1));
	{
		var *file_var = get_var("HISTFILE");
		char *filepath = file_var == NULL ? NULL : file_var->val;
		char *line;
		int handle = open(filepath, O_RDONLY);
		size_t read_size;

		if (handle == -1)
			return (NULL);

		do {
			getln(&line, &read_size, handle);
			if (read_size > 0)
			{
				hist_pstack(line, 1, 1);
			}
			else
			{
				drop(line);
			}
		} while (read_size > 0);

		loaded = 1;
		close(handle);
		return (hist_pstack(NULL, 1, 1));
	}
}

/**
 * save_history - saves the history stack to the history file
 *
 * Return: the history stack
 */
char **save_history()
{
	var *file_var = get_var("HISTFILE");
	char *filepath = file_var == NULL ? NULL : file_var->val;
	char **history = hist_pstack(NULL, 0, 1);
	char *str = NULL;
	int handle = open(filepath, O_WRONLY | O_CREAT, 00777);
	uint history_i = 0;

	if (handle == -1)
		return (NULL);
	for (history_i = 0; history[history_i] != NULL; ++history_i)
	{
		str = history[history_i];
		write(handle, str, str_len(str));
		if (!str_ends_with(history[history_i], "\n"))
			write(handle, "\n", 1);
	}
	close(handle);
	return (history);
}

/**
 * builtin__history - handles the `history` command (shows command history)
 * @params: command arguments
 *
 * Return: command exit status
 */
int builtin__history(Invokable params)
{
	char **args __attribute__((unused)) = params.args;
	char **history = hist_pstack(NULL, 0, 1);
	char *history_i_str = NULL;
	char *blank_fill;
	uint history_i = 0;

	if (*args != NULL)
	{
		put_e("history: too many arguments\n");
		return (1);
	}
	for (history_i = 0; history[history_i] != NULL; ++history_i)
	{
		history_i_str = unum_to_str(history_i);
		blank_fill = str_fill(' ', 5 - str_len(history_i_str));
		put_s(blank_fill);
		put_s(history_i_str);
		put_s("  ");
		put_s(history[history_i]);
		if (!str_ends_with(history[history_i], "\n"))
			put_s("\n");
		drop(history_i_str);
		drop(blank_fill);
	}
	return (0);
}
