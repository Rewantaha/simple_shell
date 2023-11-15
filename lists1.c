#include "shell.h"

/**
 * my_list_len - determines length of linked list
 * @my_h: pointer to first node
 *
 * Return: size of list
 */
size_t my_list_len(const list_t *my_h)
{
	size_t i = 0;

	while (my_h)
	{
		my_h = my_h->next;
		i++;
	}
	return (i);
}

/**
 * my_list_to_strings - returns an array of strings of the my_list->str
 * @my_head: pointer to first node
 *
 * Return: array of strings
 */
char **my_list_to_strings(list_t *my_head)
{
	list_t *my_node = my_head;
	size_t i = my_list_len(my_head), j;
	char **my_strs;
	char *my_str;

	if (!my_head || !i)
		return (NULL);
	my_strs = malloc(sizeof(char *) * (i + 1));
	if (!my_strs)
		return (NULL);
	for (i = 0; my_node; my_node = my_node->next, i++)
	{
		my_str = malloc(my_strlen(my_node->str) + 1);
		if (!my_str)
		{
			for (j = 0; j < i; j++)
				free(my_strs[j]);
			free(my_strs);
			return (NULL);
		}

		my_str = my_strcpy(my_str, my_node->str);
		my_strs[i] = my_str;
	}
	my_strs[i] = NULL;
	return (my_strs);
}

/**
 * my_print_list - prints all elements of a list_t linked list
 * @my_h: pointer to first node
 *
 * Return: size of list
 */
size_t my_print_list(const list_t *my_h)
{
	size_t i = 0;

	while (my_h)
	{
		my_puts(convert_number(my_h->num, 10, 0));
		my_putchar(':');
		my_putchar(' ');
		my_puts(my_h->str ? my_h->str : "(nil)");
		my_puts("\n");
		my_h = my_h->next;
		i++;
	}
	return (i);
}

/**
 * my_node_starts_with - returns node whose string starts with prefix
 * @my_node: pointer to list head
 * @my_prefix: string to match
 * @my_c: the next character after prefix to match
 *
 * Return: match node or null
 */
list_t *my_node_starts_with(list_t *my_node, char *my_prefix, char my_c)
{
	char *my_p = NULL;

	while (my_node)
	{
		my_p = starts_with(my_node->str, my_prefix);
		if (my_p && ((my_c == -1) || (*my_p == my_c)))
			return (my_node);
		my_node = my_node->next;
	}
	return (NULL);
}

/**
 * my_get_node_index - gets the index of a node
 * @my_head: pointer to list head
 * @my_node: pointer to the node
 *
 * Return: index of node or -1
 */
ssize_t my_get_node_index(list_t *my_head, list_t *my_node)
{
	size_t i = 0;

	while (my_head)
	{
		if (my_head == my_node)
			return (i);
		my_head = my_head->next;
		i++;
	}
	return (-1);
}

