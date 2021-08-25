#include "main.h"
#include <stdlib.h>
#include <inttypes.h>

/**
 * htbl_str_get - gets the value assiciated with a string key from a hash table
 * @htbl: hashtable to query
 * @key: string key to search for
 *
 * Return: the pointer to the value
 */
void *htbl_str_get(Hashtable *htbl, char *key)
{
	return (htbl_get(htbl, key, str_len(key)));
}

/**
 * mk_hnode - creates a hashitem node
 * @key: key of hashitem
 * @key_len: size of `key`
 * @obj: value to associate `key` with
 *
 * Return: the node created
 */
static Node *mk_hnode(void *key, uint key_len, void *obj)
{
	Node *obj_node = make_big_node(HashItem, key_len);
	HashItem *obj_item = (HashItem *)obj_node->val;

	obj_item->key = (char *)obj_item + sizeof(HashItem);
	memcp(key, obj_item->key, key_len);
	obj_item->key_len = key_len;
	obj_item->val = obj;
	obj_node->val = obj_item;
	return (obj_node);
}

/**
 * htbl_set - associates a key with a value in a hashtable
 * @tbl: a hashtable
 * @key: key to use
 * @key_len: size of `key`
 * @obj: value to associate `key` with
 *
 * Return: 1 if the pairs were associated, else 0
 */
char htbl_set(Hashtable *tbl, void *key, uint key_len, void *obj)
{
	HashEntry *entry = htbl_find(tbl, key, key_len);
	Node *obj_node = NULL;
	Node *node = entry->first;
	HashItem *node_item;

	if (node == NULL)
	{
		obj_node = mk_hnode(key, key_len, obj);
		entry->first = obj_node;
	}
	else
		while (1)
		{
			node_item = node->val;
			if (key_cmp(node_item, key, key_len))
			{
				((HashItem *)node->val)->val = obj;
				break;
			}
			else if (node->next == NULL)
			{

				obj_node = mk_hnode(key, key_len, obj);
				node->next = obj_node;
				break;
			}
			node = node->next;
		}
	return (1);
}

/**
 * htbl_rm - removes a key-value association from a hashtable
 * @htbl: a hashtable
 * @key: key of pair to remove
 * @key_len: size of `key`
 *
 * Return: the value of the removed pair
 */
void *htbl_rm(Hashtable *htbl, void *key, uint key_len)
{
	HashEntry *entry = htbl_find(htbl, key, key_len);
	Node *prev_node = NULL, *current_node = entry->first;
	HashItem *item;
	void *val;

	for (; current_node != NULL; prev_node = current_node,
			 current_node = current_node->next)
	{
		item = current_node->val;
		if (key_cmp(item, key, key_len))
		{
			val = item->val;
			if (prev_node == NULL)
				entry->first = current_node->next;
			else
				prev_node->next = current_node->next;
			free_node(current_node);
			return (val);
		}
	}
	return (NULL);
}

/**
 * htbl_node_iter - gets the next node in the same/different entry of `htbl`
 * @htbl: a hashtable
 * @prev: previous node
 *
 * Return: the next node if any, else NULL
 */
Node *htbl_node_iter(Hashtable *htbl, Node *prev)
{
	HashItem *prev_item;
	HashEntry *entry;
	uint entry_i;


	if (prev != NULL)
	{
		prev_item = prev->val;
		entry = htbl_find(htbl, prev_item->key, prev_item->key_len);
		entry_i = ((char *)entry - (char *)htbl->entries) / sizeof(HashEntry) + 1;
	}
	else
		entry_i = 0;
	if (prev != NULL && prev->next != NULL)
		return (prev->next);

	for (; entry_i < htbl->size; entry_i++)
	{
		if ((htbl->entries + entry_i)->first != NULL)
			return ((htbl->entries + entry_i)->first);
	}
	return (NULL);
}
