#include "main.h"
#include <unistd.h>
#include <limits.h>

/**
 * put_prompt - writes the $PS1 prompt to the standard output
 *
 * Return: void
 */
void put_prompt(void)
{
	char *PS1, interactive, *datum, c_tmp[] = "0";
	var *PS1_obj;
	Hashtable *env_htbl = glob_g(VAR_ENV);
	int *file_handle = repl_file(NULL, 0), PS1_len;
	uint ps1_i = 0;

	interactive = file_handle == NULL ? 0 : isatty(*file_handle);
	PS1_obj = interactive ? htbl_str_get(env_htbl, "PS1") : NULL;
	PS1 = PS1_obj == NULL ? "" : PS1_obj->val;

	PS1_len = str_len(PS1);
	for (ps1_i = 0; (int)ps1_i < PS1_len;)
	{
		if (PS1[ps1_i] == '$')
		{
			datum = parse_var(PS1 + ps1_i, &ps1_i);
			put_s(datum);
			drop(datum);
		}
		else
		{
			c_tmp[0] = PS1[ps1_i];
			put_s(c_tmp);
			++ps1_i;
		}
	}
}

/**
 * validate_statements - validates statements
 * @rstms: stms reference
 * @rstm_i: stm_i reference
 * @rcont_0: cont_0 reference
 * @rstm: rstm reference
 * @rcontinuation: continuation reference
 *
 * Return: void
 */
void validate_statements(RawStatement ***rstms, int *rstm_i, char *rcont_0,
						 RawStatement **rstm, char *rcontinuation)
{
	char cont_0 = *rcont_0, continuation = *rcontinuation;
	RawStatement **stms = *rstms, *stm = *rstm;
	int stm_i = *rstm_i;

	for (stm_i = 0; stms[stm_i] != NULL; ++stm_i)
	{
		stm = stms[stm_i];
		if (stm->datums[0] == NULL && stm->delim != DELIM_END)
		{
			put_ef("Syntax error: \""),	put_e(delim_to_str(stm->delim));
			if (stm_i != 0 && stm->delim == DELIM_STATEMENT)
				put_e(";");
			put_e("\" unexpected\n"), set_last_status(2), cont_0 = 1;
			break;
		}
		else if ((stm->delim == DELIM_OR || stm->delim == DELIM_AND) &&
				 (stms[stm_i + 1] == NULL || stms[stm_i + 1]->datums[0] == NULL))
			continuation = 1, cont_0 = 1;
	}
	*rstms = stms, *rstm_i = stm_i, *rcont_0 = cont_0, *rstm = stm,
		*rcontinuation = continuation;
}

/**
 * run_statements - executes statements
 * @rstms: stms reference
 * @rstm_i: stm_i reference
 * @rstm: stm reference
 * @rskip_stms: skip_stms reference
 * @rstatus: status reference
 *
 * Return: void
 */
void run_statements(RawStatement ***rstms, int *rstm_i, RawStatement **rstm,
					char *rskip_stms, int *rstatus)
{
	RawStatement **stms = *rstms, *stm = *rstm;
	char skip_stms = *rskip_stms;
	int stm_i = *rstm_i, status = *rstatus;

	skip_stms = 0;
	for (stm_i = 0; stms[stm_i] != NULL; ++stm_i)
	{
		stm = stms[stm_i];
		if (sig_int(0, 0) == 1)
		{
			sig_int(0, 1);
			break;
		}
		if (skip_stms && stm->delim == DELIM_STATEMENT)
		{
			skip_stms = 0;
			continue;
		}
		if (skip_stms)
			continue;
		status = interprete_statement(stm);
		set_last_status(status);
		if ((stm->delim == DELIM_AND && status != 0) ||
			(stm->delim == DELIM_OR && status == 0))
			skip_stms = 1;
	}
	*rstms = stms, *rstm_i = stm_i, *rstm = stm, *rskip_stms = skip_stms,
		*rstatus = status;
}

/**
 * check_continuation - checks whether the input is complete
 * @rcontinuation: continuation reference
 * @rcont: cont reference
 * @rinput: input reference
 * @rinput_len: input_len reference
 * @rcont_len: cont_len reference
 * @rhandle: handle reference
 *
 * Return: 1 if there should b a continuation, else 0
 */
char check_continuation(char *rcontinuation, char **rcont, char **rinput,
						size_t *rinput_len, size_t *rcont_len, int *rhandle)
{
	char continuation = *rcontinuation, *cont = *rcont, *input = *rinput;
	size_t input_len = *rinput_len, cont_len = *rcont_len;
	int handle = *rhandle;

	if (continuation)
	{
		put_s("> "), getln(&cont, &cont_len, handle);
		input = str_append(input, cont), input_len += cont_len;
		if (*cont == '\0')
		{
			put_ef("Syntax error: end of file unexpected\n");
			set_last_status(2),	continuation = 0;
			return (1);
		}
		else if (*cont == '\n')
			return (1);
	}
	else
	{
		put_prompt(), getln(&input, &input_len, handle);
		if (*input == '\0')
			put_s(""), drop(input), quit(INT_MIN);
		else if (*input == '\n')
		{
			drop(input);
			return (1);
		}
	}
	*rcontinuation = continuation, *rcont = cont, *rinput = input,
		*rinput_len = input_len, *rcont_len = cont_len, *rhandle = handle;
	return (0);
}

/**
 * start_repl - starts a shell repl on the given handle
 * @handle: file handle to get inputs from
 *
 * Return: the exit status of the last statement
 */
int start_repl(int handle)
{
	char *input, *cont, continuation = 0, cont_0 = 0,
		skip_stms = 0, interactive = isatty(handle), *_exp;
	size_t input_len = 0, cont_len = 0;
	RawStatement **stms = NULL, *stm, **_stms;
	int stm_i = 0, status = -1, cmd_num = 0;

	(void)interactive;
	do {
		++cmd_num, cmd_no(cmd_num, 1), repl_file(&handle, 1), sig_int(0, 1);

		if (check_continuation(&continuation, &cont, &input, &input_len,
							   &cont_len, &handle))
			continue;

		hist_pstack(str_clone(input), 0, 0), free_raw_statements(stms);
		/* if (interactive) */
		_stms = parse_raw_statements(input), _exp = expand_aliases(_stms, NULL),
			stms = parse_raw_statements(_exp), free_raw_statements(_stms), drop(_exp);
		/* else */
		/* stms = parse_raw_statements(input); */
		drop(input), expose_repl_statements(stms), continuation = 0, cont_0 = 0;
		validate_statements(&stms, &stm_i, &cont_0, &stm, &continuation);
		if (cont_0)
			continue;
		run_statements(&stms, &stm_i, &stm, &skip_stms, &status);
	} while (1);
	return (status);
}
