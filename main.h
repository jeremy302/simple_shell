#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

/**
 * enum DELIMITER - statements delimiter
 * @DELIM_NONE: invalid delimiter
 * @DELIM_OR: || delimiter
 * @DELIM_AND: && delimiter
 * @DELIM_STATEMENT: ; delimiter
 * @DELIM_END: \0 or \n delimiter
 */
typedef enum DELIMITER
{
	DELIM_NONE,
	DELIM_OR,
	DELIM_AND,
	DELIM_STATEMENT,
	DELIM_END
} DELIMITER;

/**
 * struct RawStatement - a parse statement which hasn't been expanded
 * @datums: words in the statement
 * @delim: the delimiter that ends the statement
 */
typedef struct RawStatement
{
	char **datums;
	DELIMITER delim;
} RawStatement;

/**
 * struct Node - a linked-list node
 * @val: value of node
 * @next: next node
 */
typedef struct Node
{
	void *val;
	struct Node *next;
} Node;


/**
 * struct var - an environment variable
 * @global: whether the variable is global or local
 * @name: the variable's name
 * @val: the variable's value
 */
typedef struct var
{
	char global;
	char *name;
	char *val;
} var;
/**
 * struct HashItem - a hashed item
 * @key: item key
 * @key_len: size of item key
 * @val: item value
 */
typedef struct HashItem
{
	void *key;
	uint key_len;
	void *val;
} HashItem;

/**
 * struct HashEntry - an entry into a hashtable
 * @first: first node in the entry
 */
typedef struct HashEntry
{
	Node *first;
} HashEntry;

/**
 * struct Hashtable - a hashtable
 * @size: size of the hashtable
 * @entries: entries in the hashtable
 */
typedef struct Hashtable
{
	uint size;
	HashEntry *entries;
} Hashtable;

/**
 * struct Invokable - parameters needed to invoke a command
 * @cmd: command name
 * @args: command arguments
 */
typedef struct Invokable
{
	char *cmd;
	char **args;
} Invokable;

typedef int (*CommandHandler) (Invokable);

/**
 * struct BuiltinHandler - a name-handler association for builtin commands
 * @name: command name
 * @handler: command handler
 */
typedef struct BuiltinHandler
{
	char *name;
	CommandHandler handler;
} BuiltinHandler;

/**
 * enum Glob_t - global variables
 * @VAR_ENV: environment variables
 * @VAR_ALIASES: aliases
 * @VAR_ALLOCS: allocations
 * @VAR_BUILTINS: builtins
 */
typedef enum Glob_t
{
	VAR_ENV,
	VAR_ALIASES,
	VAR_ALLOCS,
	VAR_BUILTINS
} Glob_t;

/**
 * enum CMD_T - command type
 * @CMD_BUILTIN: a builtin comman
 * @CMD_PATH: a path command
 * @CMD_DIR: a file nae
 */
typedef enum CMD_T
{
	CMD_BUILTIN,
	CMD_PATH,
	CMD_DIR
} CMD_T;

/**
 * struct HELP_DATA - help data for builtin commands
 * @cmd: the command
 * @data: help text
 */
typedef struct HELP_DATA
{
	char *cmd;
	char *data[30];
} HELP_DATA;

typedef void (*val_freer)(void *);

#define time_init(key) clock_t time_##key, diff_##key
#include <time.h>
#define time_start(key) (time_##key = clock())
#define time_stop(key) (diff_##key = clock() - time_##key,				\
						printf("\nTIME (" #key ") : %lu ms\n", \
							   (diff_##key * 1000) / (CLOCKS_PER_SEC)))
#define take(r) ti##r
#define gm(r) gm##r
#define stt(k) struct tm
#define ht(tt) tt->tm_hour
#define dt(tt) tt->tm_mday

/* mem */
#define enpick(cond, ptr, type, size, inc)		{	\
	if ((cond) >= size)\
		ptr = repick(ptr, sizeof(type) * size, sizeof(type) * (size + inc)), \
			size += inc; }

void *zalloc(size_t size);
void *pick(size_t size);
void *repick(void *ptr, unsigned int old_size, unsigned int new_size);
void *drop(void *ptr);
void dropv(void *ptr);
void drop_2d(void **arr);
void drop_all(void);

Node *gc_mk_hnode(void *key, uint key_len, void *obj);
char gc_htbl_set(Hashtable *tbl, void *key, uint key_len, void *obj);
void *gc_htbl_rm(Hashtable *htbl, void *key, uint key_len);


char memeq(void *obj_1, void *obj_2, uint len);
void memcp(void *src, void *dst, uint len);
void *mem_clone(void *src, uint len);
void *mem_fill(char val, uint len);
void mem_move(void *src, void *dst, uint len);
void mem_set(void *ptr, char v, uint len);

char is_digit(char c);
char is_lower(char c);
char is_upper(char c);
char is_letter(char c);
size_t str_len(char *str);
char *str_clone(char *str);
char *str_chr(char *str, char chr);
char **str_split(char *str, char delim, int max_splits);
char str_ends_with(char *str, char *sub_str);
char str_starts_with(char *str, char *sub_str);
char *str_append(char *dst, char *src);
char str_eq(char *s1, char *s2);
char str_has_ch(char *str, char c);
char *str_fill(char c, uint len);
char chr_printable(char c);
char str_contains(char *str, char *sub_str);

/* char *readln(int handle); */
int getln(char **lineptr, size_t *n, int handle);
size_t put_s(char *str);
size_t put_e(char *str);
size_t put_ef(char *str);
char *num_to_str(long num);
char *unum_to_str(ulong num);

#define make_node(type) (new_node(sizeof(type)))
#define make_big_node(type, extra) (new_node(sizeof(type) + extra))
Node *new_node(uint val_size);
void free_node(Node *node);
void iter_node(Node *node, void (*f)(Node *));

#define htbl_sizeof(size) (sizeof(Hashtable) + (size * sizeof(HashEntry)))

Hashtable *init_htbl(void *htbl, uint size);
Hashtable *make_htbl(uint size);
void free_htbl(Hashtable *htbl, val_freer freer);
void free_static_htbl(Hashtable *htbl, val_freer freer);
char key_cmp(HashItem *item, void *key, uint key_len);
HashEntry *htbl_find(Hashtable *tbl, void *key, uint key_len);
void *htbl_get(Hashtable *tbl, void *key, uint key_len);
char htbl_set(Hashtable *tbl, void *key, uint key_len, void *obj);
void *htbl_str_get(Hashtable *htbl, char *key);
void *htbl_rm(Hashtable *htbl, void *key, uint key_len);
Node *htbl_node_iter(Hashtable *htbl, Node *prev);

void *glob_g(Glob_t type);
void free_globals(void);
RawStatement **expose_repl_statements(RawStatement **statements);
var *mk_var(char *name, char *val);
var *get_var(char *name);
void set_last_status(int status);
var *set_var(char *name, char *val);
void free_var(var *obj);
void free_aliases_ls(void);

char reg_env(void);
char **set_env(char *name, char *value);
void free_env(void);

void _exit(int status);

char any_char_in_str(char *str, char *delims);
char *iter_tokens(char *str, char *delims);

char *trim_spcs(char *stream, uint *cursor);
char *parse_var(char *str, uint *cursor);
char *parse_sq_str(char *str, uint *cursor);
char *parse_dq_str(char *str, uint *cursor);
char *parse_datum(char *str, uint *cursor);
void parse_statement(int handle);
DELIMITER parse_delimiter(char *str, uint *cursor);
char **parse_raw_datums(char *str, uint *cursor);
RawStatement **parse_raw_statements(char *str);
char *delim_to_str(DELIMITER delim);
char *expand_aliases(RawStatement **statements, Hashtable *expanded_htbl);
void free_raw_statements(RawStatement **statements);

pid_t *current_exec(pid_t *pid, char force_set);
int exec(char *pathname, char *args[]);
int exec_path_cmd(Invokable params);
int exec_dir_cmd(Invokable params);
char *join_paths(char *s1, char *s2);

char *search_path(char *str);

int start_repl(int handle);
int interprete_statement(RawStatement *statement);
int *repl_file(int *handle, char force_set);
uint cmd_no(uint val, char set);
void put_prompt(void);

int builtin__cd(Invokable params);
int builtin__env(Invokable params);
int builtin__setenv(Invokable params);
int builtin__unsetenv(Invokable params);

int builtin__alias(Invokable params);
Node *alias_ls(char *name);

int builtin__history(Invokable params);
char **load_history();
char **save_history();
char **hist_pstack(char *ln, char is_old, char get_old);
void free_history(void);

int builtin__help(Invokable params);

int builtin__exit(Invokable params);
int parse_exit_status(char *str, char *success);

void sigint_handler(int sig);
int sig(int c);
char sig_int(char val, char set);

void quit(int status);
void init(char **args);
char **get_env(char **env, char set);
char **prog_args(char **args, char set);


#endif /* MAIN_H */
