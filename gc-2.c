#include "main.h"

/**
 * zalloc - allocates memory and sets all bytes to 0
 * @size: size of memory
 *
 * Return: the allocated memory
 */
void *zalloc(size_t size)
{
	char *ptr = malloc(size);

	if (ptr != NULL)
	{
		while (size-- > 0)
			ptr[size] = 0;
	}
	return (ptr);
}

/**
 * mk_hnode - creates a hashitem node
 * @key: key of hashitem
 * @key_len: size of `key`
 * @obj: value to associate `key` with
 *
 * Return: the node created
 */
Node *gc_mk_hnode(void *key, uint key_len, void *obj)
{
	Node *obj_node;
	HashItem *obj_item;

	obj_node = zalloc(sizeof(Node) + sizeof(HashItem) + key_len + 2);
	obj_node->val = (char *)obj_node + sizeof(Node);

	obj_item = (HashItem *)obj_node->val;

	obj_item->key = (char *)obj_item + sizeof(HashItem);
	memcp(key, obj_item->key, key_len);
	obj_item->key_len = key_len;
	obj_item->val = obj;
	obj_node->val = obj_item;
	return (obj_node);
}

/**
 * gc_htbl_set - associates a key with a value in a hashtable
 * @tbl: a hashtable
 * @key: key to use
 * @key_len: size of `key`
 * @obj: value to associate `key` with
 *
 * Return: 1 if the pairs were associated, else 0
 */
char gc_htbl_set(Hashtable *tbl, void *key, uint key_len, void *obj)
{
	HashEntry *entry = htbl_find(tbl, key, key_len);
	Node *obj_node = NULL;
	Node *node = entry->first;
	HashItem *node_item;

	if (node == NULL)
	{
		obj_node = gc_mk_hnode(key, key_len, obj);
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

				obj_node = gc_mk_hnode(key, key_len, obj);
				node->next = obj_node;
				break;
			}
			node = node->next;
		}
	return (1);
}

/**
 * gc_htbl_rm - removes a key-value association from a hashtable
 * @htbl: a hashtable
 * @key: key of pair to remove
 * @key_len: size of `key`
 *
 * Return: the value of the removed pair
 */
void *gc_htbl_rm(Hashtable *htbl, void *key, uint key_len)
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
			/* printf("found\n"); */
			val = item->val;
			if (prev_node == NULL)
				entry->first = current_node->next;
			else
				prev_node->next = current_node->next;
			free(current_node);
			return (val);
		}
	}
	return (NULL);
}
