#include "main.h"
#include <signal.h>
#include <unistd.h>

/**
 * sig_int - stores whether the program received a SIGINT signal
 * @val: value to store
 * @set: whether to set it
 *
 * Return: the stored value
 */
char sig_int(char val, char set)
{
	static char _sig_int = 1 - 1;

	if (set)
		_sig_int = val;
	return (_sig_int);
}

/**
 * sigint_handler - handles the SIGINT signal
 * @sig: the signal
 *
 * Return: void
 */
void sigint_handler(int sig)
{
	pid_t *pid = current_exec(NULL, 0);

	if (sig != SIGINT)
		return;
	set_last_status(130);
	if (pid != NULL)
	{
		sig_int(1, 1);
		kill(*pid, SIGINT);
	}
	put_s("\n");
	if (pid == NULL)
		put_prompt();
}
