#include "main.h"

#define make_node(type) (new_node(sizeof(type)))
#define make_big_node(type, extra) (new_node(sizeof(type) + extra))

/**
 * new_node - creates a node
 * @val_size: extra size to allocate with the node (to be freed at once)
 *
 * Return: the node created
 */
Node *new_node(uint val_size)
{
	Node *node = pick(sizeof(Node) + val_size);

	if (val_size)
		node->val = (char *)node + sizeof(Node);
	return (node);
}

/**
 * free_node - frees a node
 * @node: node to free
 *
 * Return: void
 */
void free_node(Node *node)
{
	drop(node);
}

/**
 * iter_node - iterates a list
 * @node: head of list
 * @f: iter function
 *
 * Return: void
 */
void iter_node(Node *node, void (*f)(Node *))
{
	Node *next;

	while (node != NULL)
		next = node->next, f(node), node = next;
}
