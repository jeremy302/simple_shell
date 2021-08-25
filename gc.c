#include "main.h"

/**
 * pick - allocates memory and fills it with zero
 * @size: size of memory (in bytes) to allocate
 *
 * Return: pointer to the memory
 */
void *pick(size_t size)
{
	/* static int i = 0; */
	/* int ii = i++; */
	/* char d __attribute__((unused)) = printf("picking: %ld(%d)\n", size, ii); */
	Hashtable *allocs_htbl = glob_g(VAR_ALLOCS);
	char *ptr = zalloc(size);

	if (ptr != NULL)
	{
		gc_htbl_set(allocs_htbl, &ptr, sizeof(char *), ptr);
	}
	/* printf("picked: %ld(%d)\n", size, ii); */
	return (ptr);
}

/**
 * drop - drops memory
 * @ptr: memory to drop
 *
 * Return: the memory dropped
 */
void *drop(void *ptr)
{
	/* static int i = 0; */
	/* int ii = i++; */
	/* char d __attribute__((unused)) = printf("dropping: %d\n", ii); */
	Hashtable *allocs_htbl = glob_g(VAR_ALLOCS);
	void *_ptr = NULL;

	_ptr = htbl_get(allocs_htbl, &ptr, sizeof(void *));

	if (_ptr == NULL)
		return (NULL);
	/* printf("==: %d\n", _ptr == ptr); */
	free(ptr);
	gc_htbl_rm(allocs_htbl, &ptr, sizeof(void *));
	/* printf("dropped: %d\n", ii); */
	return (ptr);
}

/**
 * dropv - drops memory and returns void
 * @ptr: memory to drop
 *
 * Return: void
 */
void dropv(void *ptr)
{
	drop(ptr);
}

/**
 * drop_2d - drops a 2d array
 * @arr: array to drop
 *
 * Return: void
 */
void drop_2d(void **arr)
{
	uint i = 0;

	if (arr == NULL)
		return;
	for (i = 0; arr[i] != NULL; ++i)
		drop(arr[i]);
	drop(arr);
}

/**
 * drop_all - drops all recorded memory
 *
 * Return: void
 */
void drop_all(void)
{
	Hashtable *allocs_htbl = glob_g(VAR_ALLOCS);
	Node *node = NULL, *_node __attribute__((unused)) = NULL;
	uint i = 0;

	for (i = 0; i < allocs_htbl->size; ++i)
	{
		node = (allocs_htbl->entries + i)->first;
		if (node == NULL)
			continue;
		while (node != NULL)
			_node = node->next, free(((HashItem *)node->val)->val),
				free(node), node = _node;
	}
	return;
	node = htbl_node_iter(allocs_htbl, node);
	while (node != NULL)
	{
		free(*(void **)((HashItem *)node->val)->key);
		_node = node;
		node = htbl_node_iter(allocs_htbl, node);
		/* free(_node); */
	}
}
