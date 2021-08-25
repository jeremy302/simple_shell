#include "main.h"
#include <stdlib.h>

/**
 * mem_fill - creates a block of memory, filled with a given byte
 * @val: byte to fill with
 * @len: size of block
 *
 * Return: the block's pointer
 */
void *mem_fill(char val, uint len)
{
	char *ptr = pick(len);

	while (len--)
		ptr[len] = val;
	return (ptr);
}

/**
 * mem_move - moves a block of memory to another locatio
 * @src: block to move
 * @dst: location to move to
 * @len: size of `src`
 *
 * Return: void
 */
void mem_move(void *src, void *dst, uint len)
{
	uint i = 0;

	for (; i < len; ++i)
		((char *)dst)[i] = ((char *)src)[i];
}

/**
 * mem_set - fills a memory block with a given value
 * @ptr: block to fill
 * @v: byte to fill with
 * @len: size of the block
 *
 * Return: void
 */
void mem_set(void *ptr, char v, uint len)
{
	while (len--)
		((char *)ptr)[len] = v;
}
