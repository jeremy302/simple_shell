#include "main.h"
#include <stdlib.h>
#include <inttypes.h>

/**
 * init_htbl - initializes a hash table
 * @htbl: hashtable
 * @size: hasntable size
 *
 * Return: the initialized hashtable
 */
Hashtable *init_htbl(void *htbl, uint size)
{
	Hashtable *tbl = htbl;

	tbl->size = size;
	tbl->entries = (void *)(((char *)tbl) + sizeof(Hashtable));
	return (tbl);
}

/**
 * make_htbl - creates a hashtable
 * @size: size of the hashtable
 *
 * Return: the hashtable created
 */
Hashtable *make_htbl(uint size)
{
	Hashtable *tbl = pick(htbl_sizeof(size));
/* pick(sizeof(Hashtable) + (size * sizeof(HashEntry))); */

	return (init_htbl(tbl, size));
}

/**
 * free_htbl - frees a hashtable
 * @htbl: a hashtable
 * @val_freer: function to use to free each item in the hashtable
 *
 * Return: void
 */
void free_htbl(Hashtable *htbl, void (*val_freer)(void *))
{
	Node *prev = NULL, *_prev;

	prev = htbl_node_iter(htbl, prev);
	while (prev != NULL)
	{
		_prev = prev;
		prev = htbl_node_iter(htbl, prev);
		if (val_freer != NULL)
			val_freer(((HashItem *)_prev->val)->val);
		free_node(_prev);
	}
	drop(htbl);
}

/**
 * free_static_htbl - frees a static hashtable
 * @htbl: a static hashtable
 * @val_freer: function to use to free each item in the hashtable
 *
 * Return: void
 */
void free_static_htbl(Hashtable *htbl, void (*val_freer)(void *ptr))
{
	Node *prev = NULL, *_prev;
	uint i = 0;

	(void) i;
	prev = htbl_node_iter(htbl, prev);
	while (prev != NULL)
	{
		_prev = prev;
		prev = htbl_node_iter(htbl, prev);
		if (val_freer != NULL)
			val_freer(((HashItem *)_prev->val)->val);
		free_node(_prev);
	}
}
