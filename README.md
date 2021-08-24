# Simple Shell

This is a simple shell, written in C by Jeremy Ola & Hezekiah Ibok. This
shell supports basic operations, such as boolean statements logic
operators (&& and ||), variables, variables and singly/double quotes
expansions.

## Features

| Feature                        | Description                                                                     |
| ------------------------------ | ------------------------------------------------------------------------------- |
| Executable files               | it allows executable files to be run                                            |
| Path Search                    | when you input a command, all directories in \`$PATH\` are searched             |
| Command arguments              | You can pass multiple arguments to all kinds of commands                        |
| Single-quote expansions        | Single-quote strings are expanded                                               |
| Double-quote expansions        | Double-quote strings are expanded                                               |
| Variables and expansions       | Variable expansions are supported                                               |
| Environment Variables          | You can add and remove environment variables                                    |
| Ctrl + C (<sup>C</sup>/SIGINT) | Intterupts are handled                                                          |
| Ctrl + D (EOF)                 | EOF conditions are handled                                                      |
| Non-interactive mode           | You can pipe input to the shell, and it will run non-interactively              |
| Script files                   | You can provide a file argument to the shell for it to be run non-interactively |
| PS1 prompt                     | the PS1 variable can be inherited, and is expanded as the shell prompt          |

## Builtins

There are a number of builtins that the shell supports

| Command  | Description                    | Example                         |
| -------- | ------------------------------ | ------------------------------- |
| cd       | change directory               | cd $HOME                        |
| alias    | create an alias                | alias L='ls'; L; alias; alias L |
| env      | Print environment variables    | env                             |
| setenv   | Set an environment variable    | setenv LL 'ls -l'; $LL          |
| unsetenv | Remove an environment variable | unsetenv LL                     |
| history  | Shows input history            | history                         |
| exit     | exit the shell                 | exit; exit 5;                   |
| help     | Dislays usage for builtins     | help cd; help                   |

## Extra Info

### Bugs

Only known bug is the spanning of unfinished input (e.g unterminated
quotes) over multiple lines.

### Authors

  - Jeremy Ola
  - Hezekiah Ibok

### Date Released

25th August 2021
