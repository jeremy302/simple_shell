#include "main.h"
#include <unistd.h>

#define READ_SIZE (1 * 5000)
#define READ_BUF_SIZE (1 * 5000 + 3)

/**
 * getln - reads a line from a file handle
 * @lineptr: pointer to a string that will be set to point to the line
 * @n: pointer to size_t, to set as the number of bytes read
 * @handle: file to read from
 *
 * Return: the number of bytes read
 */
int getln(char **lineptr, size_t *n, int handle)
{
	static char buf[READ_BUF_SIZE] = {0};
	uint buf_i, read_len = 1;
	char *str = pick(1), *_str, buf_tmp;

	while (read_len != 0)
	{
		/* if (handle == 0) */
		/* printf("getting input\n"); */
		buf_i = str_chr(buf, '\n') - buf;
		if (buf[buf_i] == '\n')
		{
			buf_tmp = buf[buf_i + 1];
			buf[buf_i + 1] = '\0';
			_str = str;
			str = str_append(str, buf);
			drop(_str);
			buf[buf_i + 1] = buf_tmp;
			mem_move(buf + buf_i + 1, buf, READ_BUF_SIZE - buf_i - 1);
			break;
		}
		else
		{
			_str = str;
			str = str_append(str, buf);
			drop(_str);
			mem_set(buf, 0, READ_BUF_SIZE);
			/* if (handle == 0) */
			/* printf("readding\n"); */
			read_len = read(handle, buf, READ_SIZE);
		}
	}
	*lineptr = str;
	*n = str_len(str);
	return (*n);
}

/**
 * put_ - writes a string to the given file handle
 * @handle: a file handle
 * @str: string to write
 *
 * Return: the number of bytes written
 */
size_t put_(int handle, char *str)
{
	return (write(handle, str, str_len(str)));
}

/**
 * put_s - writes a string to the standard output
 * @str: string to write
 *
 * Return: the number of bytes written
 */
size_t put_s(char *str)
{
	return (put_(STDOUT_FILENO, str));
}

/**
 * put_e - writes a string to the standard error
 * @str: string to write
 *
 * Return: the number of bytes written
 */
size_t put_e(char *str)
{
	return (put_(STDERR_FILENO, str));
}

/**
 * put_ef - writes a string to the standard error
 * @str: string to write
 *
 * Return: the number of bytes written
 */
size_t put_ef(char *str)
{
	size_t write_len = 0;
	char **args = prog_args(NULL, 0);
	char *cmd_no_str = unum_to_str(cmd_no(0, 0));

	write_len += put_(STDERR_FILENO, *args);
	write_len += put_(STDERR_FILENO, ": ");
	write_len += put_(STDERR_FILENO, cmd_no_str);
	write_len += put_(STDERR_FILENO, ": ");
	write_len += put_(STDERR_FILENO, str);
	drop(cmd_no_str);
	return (write_len);
}

