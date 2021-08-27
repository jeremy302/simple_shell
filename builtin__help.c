#include "main.h"

#define  BUILTIN_HELP_CD						\
"cd: cd [DIRECTORY]\n",\
"	Change the shell working directory.",\
"\n",\
"	Change the current directory to DIR. the default DIR is the value",\
"	of the HOME shell variable.",\
"\n",\
"	If the directory is not found, an error text is printed to the",\
"	standard error, and $? is set accordingly.",\
"\n",\
"	If the directory exists and can be switched to, `$OLDPWD` is set",\
"	to the previous directory, and `$PWD` is set the new directory.",\
"\n",\
"	You can go back to the previous directory by using `-` in place",\
"	of the `DIRECTORY` argument.",\
"\n",\
"	*Arguments are not expanded. (So `~` doesn't expand to the HOME",\
"	directory, and `*` isn't a wildcard). *",\
"\n",\
"	Examples: ",\
"		`cd /usr/bin`\n",\
"		`cd -`\n",\
"		`cd /home\n",\
"		`cd` - switches to the `$HOME` directory",\
"\n"

#define BUILTIN_HELP_CD_ \
"cd: cd [DIRECTORY]\n"\
"	Change the shell working directory.\n"\
"\n"\
"	Change the current directory to DIR. the default DIR is the value\n"\
"	of the HOME shell variable.\n"\
"\n"\
"	If the directory is not found, an error text is printed to the\n"\
"	standard error, and $? is set accordingly.\n"\
"\n"\
"	If the directory exists and can be switched to, `$OLDPWD` is set"\
"	to the previous directory, and `$PWD` is set the new directory.\n"\
"\n"\
"	You can go back to the previous directory by using `-` in place"\
"	of the `DIRECTORY` argument."\
"\n"\
"	*Arguments are not expanded. (So `~` doesn't expand to the HOME\n"\
"	directory, and `*` isn't a wildcard). *"\
"\n"\
"	Examples: \n"\
"		`cd /usr/bin`\n"\
"		`cd -`\n"\
"		`cd /home`\n"\
"		`cd` - switches to the `$HOME` directory"\
"\n"

#define BUILTIN_HELP_EXIT_ \
"exit: exit [status]\n"\
"	Exit the shell.\n"\
"\n"\
"	Exits the shell with a status of `status`.  If `status` is not"\
"provided, the exit status of the last executed command is used"\
	"\n"

#define BUILTIN_HELP_EXIT \
"exit: exit [status]\n",\
"	Exit the shell.",\
"\n\n",\
"	Exits the shell with a status of `status`. If `status` is not",\
"provided, the exit status of the last executed command is used",\
"\n"

#define BUILTIN_HELP_ALIAS \
"alias: alias [name[='value'] ...]",\
"\n",\
"alias: Prints a list of all aliases, one per line, in the form name='value'",\
"\nalias name [name2 ...]: Prints the aliases name, name2, etc 1 per line,",\
" in the form name='value'",\
"\nalias name='value' [...]: Defines an alias for each name whose value is",\
" given. If name is already an alias, replaces its value with value",\
"\n",\
"An alias is a datum that would be substituted for it's mapped ",\
"expansion value during preprocessing.",\
"\nOnly the first command in each statement is tested for",\
"an alias.",\
"\nIf an alias' expansion ends with a space, the next datum is tested too.",\
"\nAliases can be recursive.",\
"\nE.g:",\
"\n	'alias'",\
"\n	'alias L='ls'; L;'",\
"\n	'alias L'\n"

#define BUILTIN_HELP_ENV \
"env: env",\
"\nprints the current environment.",\
"\nonly global variables are printed and passed on to child processes.",\
"\n\nE.g: 'env'\n"

#define BUILTIN_HELP_SETENV \
"setenv: setenv VARIABLE VALUE",\
"\nsets an environment variable to the given value.",\
"\nE.g 'setenv L ls'",\
"\n"

#define BUILTIN_HELP_UNSETENV					\
"unsetenv: unsetenv VARIABLE",\
"\nremoves an environment variable.",		\
"\nE.g 'unsetenv L'",\
"\n"

#define BUILTIN_HELP_UNSETENV					\
"unsetenv: unsetenv VARIABLE",\
"\nremoves an environment variable.",		\
"\nE.g 'unsetenv L'",\
"\n"

#define BUILTIN_HELP_HISTORY					\
"history: history",\
"\nDisplays up to the last 4095 commands inputted.",		\
"\nE.g 'history'",\
"\n"

#define BUILTIN_HELP_ALL					\
"These commands are defined internally, and can only be used with this shell",\
"\n\ncd [DIRECTORY]",\
"\nalias [name[='value'] ...]",\
"\nenv",\
"\nsetenv VARIABLE VALUE",\
"\nunsetenv VARIABLE",\
"\nhistory",\
"\nexit: exit [status]",\
"\nhelp [BUILTIN]",\
"\n"

#define BUILTIN_HELP_HELP						\
"help: help [BUILTIN]",\
"\nIf the `BUILTIN` option is provided, it displays information about",	\
"the specified builtin command.",									\
"\nWithout any arguments, all builtin commands along with their syntax.",	\
"\nE.g 'help cd'",													\
"\n    'help'",														\
"\n"

/**
 * builtin__help - handles the `help` command (shows help for builtin commands)
 * @params: command arguments
 *
 * Return: command exit status
 */
int builtin__help(Invokable params)
{
	char **args = params.args;
	char *cmd = *args;
	uint data_i = 0, str_i = 0;
	HELP_DATA data[10] = {
		{ "cd",  {BUILTIN_HELP_CD, NULL} },
		{ "alias", {BUILTIN_HELP_ALIAS, NULL} },
		{ "env", {BUILTIN_HELP_ENV, NULL} },
		{ "setenv", {BUILTIN_HELP_SETENV, NULL} },
		{ "unsetenv", {BUILTIN_HELP_UNSETENV, NULL} },
		{ "history", {BUILTIN_HELP_HISTORY, NULL} },
		{ "exit", {BUILTIN_HELP_EXIT, NULL} },
		{ "help", {BUILTIN_HELP_HELP, NULL} },
		{ "", {BUILTIN_HELP_ALL, NULL}},
		{ NULL, {NULL}}
	};
	for (data_i = 0; data[data_i].data[0] != NULL; ++data_i)
	{
		HELP_DATA current = data[data_i];

		if (str_eq(cmd ==  NULL ? "" : cmd, current.cmd))
		{
			for (str_i = 0; current.data[str_i] != NULL; ++str_i)
				put_s(current.data[str_i]);
			return (0);
		}
	}

	put_ef("help: no help topics match `");
	put_e(cmd);
	put_e("'.  Try `help help'");
	put_e(".\n");
	return (1);
}
