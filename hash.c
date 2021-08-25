#include "main.h"
#include <stdlib.h>
#include <inttypes.h>

/**
 * rol32 - rotates a 32-bit integer
 * @val: value to rotate
 * @count: number of bit rotations (can't be more than 32)
 *
 * Return: the value after the rotations
 */
inline uint32_t rol32(uint32_t val, uint32_t count)
{
	return ((val << count) | (val >> (32 - count)));
}

/**
 * hash - hashes an object for a hashtble
 * @obj: object to hash
 * @len: size of `obj`
 * @seed: value to mix with the hash
 *
 * Return: the calculated hash
 */
uint32_t hash(void *obj, uint len, uint seed)
{
	/* https://en.wikipedia.org/wiki/MurmurHash */

	uint32_t c1 = 0xcc9e2d51, c2 = 0x1b873593, r1 = 15, r2 = 13;
	uint32_t hash = seed, l = len, k, m = 5, n = 0xe6546b64;

	for (; l >= 4; l -= 4, obj = (char *)obj + 4)
		k = *(uint32_t *)obj, k *= c1, k = rol32(k, r1), k *= c2,
			hash ^= k, hash = rol32(hash, r2), hash = (hash * m) + n;

	for (k = 0, obj = (char *)obj + l - 1; l > 0; l--, obj = (char *)obj - 1)
		k <<= 8, k |= *(char *)obj;
	k *= c1, k = rol32(k, r1), k *= c2;
	hash ^= k, hash ^= len, hash ^= (hash >> 16), hash *= 0x85ebca6b;
	hash ^= (hash >> 13), hash *= 0xc2b2ae35, hash ^= (hash >> 16);
	return (hash);
}

/**
 * htbl_find - finds the hash table entry for a given key
 * @tbl: hash table to query
 * @key: key of object to find
 * @key_len: size of `key`
 *
 * Return: the hash table entry that `key` maps to
 */
HashEntry *htbl_find(Hashtable *tbl, void *key, uint key_len)
{
	return (tbl->entries + (hash(key, key_len, 0) % tbl->size));
}

/**
 * key_cmp - checks if a hash item matches a given key
 * @item: a hash item
 * @key: a key
 * @key_len: size of `key`
 *
 * Return: 1 if they match, else 0
 */
inline char key_cmp(HashItem *item, void *key, uint key_len)
{
	return (item->key_len == key_len && memeq(key, item->key, key_len));
}

/**
 * htbl_get - retrieves a value from a hashtable that a given key maps to
 * @htbl: hash table to query
 * @key: key to find
 * @key_len: size of `key`
 *
 * Return: the pointer to the value
 */
void *htbl_get(Hashtable *htbl, void *key, uint key_len)
{
	HashEntry *entry = htbl_find(htbl, key, key_len);
	Node *node = entry->first;
	HashItem *item;

	for (; node != NULL; node = node->next)
	{
		item = (HashItem *)node->val;
		if (key_cmp(item, key, key_len))
			return (item->val);
	}
	return (NULL);
}

