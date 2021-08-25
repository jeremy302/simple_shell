#include "main.h"

/**
 * is_digit - determines if a character is a digit
 * @c: character to check
 *
 * Return: if `c` is a digit
 */
char is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

/**
 * is_lower - determines if a character is a lowercase letter
 * @c: character to check
 *
 * Return: if `c` is a lowercase letter
 */
char is_lower(char c)
{
	return (c >= 'a' && c <= 'z');
}

/**
 * is_upper - determines if a character is an uppercase letter
 * @c: character to check
 *
 * Return: if `c` is an uppercase letter
 */
char is_upper(char c)
{
	return (c >= 'A' && c <= 'Z');
}

/**
 * is_letter - determines if a character is a letter
 * @c: character to check
 *
 * Return: if `c` is a letter
 */
char is_letter(char c)
{
	return (is_lower(c) || is_upper(c));
}

/**
 * is_alpha - determines if a character is alphanumeric
 * @c: character to check
 *
 * Return: if `c` is alphanumeric
 */
char is_alpha(char c)
{
	return (is_letter(c) || is_digit(c));
}


