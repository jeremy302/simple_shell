#include "main.h"
#include <stdlib.h>



/**
 * repick - reallocates the block at `ptr` to a block of size: `new_size`
 * @ptr: old block
 * @old_size: size of ptr
 * @new_size: size of new block
 *
 * Return: new block
 */
void *repick(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *ptr_;
	unsigned int i = 0;

	if (ptr == NULL)
		return (pick(new_size));
	if (!new_size)
		return (drop(ptr), NULL);

	if (new_size == old_size)
		return (ptr);

	ptr_ = pick(new_size);
	if (ptr_ == NULL)
		return (NULL);
	for (; i < (old_size < new_size ? old_size : new_size); i++)
		ptr_[i] = ((char *)ptr)[i];
	drop(ptr);
	return (ptr_);
}


/**
 * memeq - checks if 2 blocks of memory are equal
 * @obj_1: first block
 * @obj_2: second block
 * @len: length to check
 *
 * Return: 1 if they are equal, else 0
 */
char memeq(void *obj_1, void *obj_2, uint len)
{
	char *_obj_1 = obj_1, *_obj_2 = obj_2;

	while (len && (*_obj_1++ == *_obj_2++))
		--len;
	return (len == 0);
}

/**
 * memcp - copies a block of memory to another location
 * @src: block to copy
 * @dst: location to copy to
 * @len: size of `src`
 *
 * Return: void
 */
void memcp(void *src, void *dst, uint len)
{
	char *_src = src, *_dst = dst;

	while (len && (*_dst++ = *_src++))
		--len;
}

/**
 * mem_clone - creates a copy of a block of memory
 * @src: block to clone
 * @len: sizeof of `src`
 *
 * Return: the new copy
 */
void *mem_clone(void *src, uint len)
{
	char *dst = pick(len);

	memcp(src, dst, len);
	return (dst);
}

