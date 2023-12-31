#include "shell.h"

/**
 * my_env - prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 *         constant function prototype.
 * Return: Always 0
 */
int my_env(my_info_t *info)
{
	my_print_list_str(info->my_env);
	return (0);
}

/**
 * my_getenv - gets the value of an environ variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *my_getenv(my_info_t *info, const char *name)
{
	list_t *node = info->my_env;
	char *p;

	while (node)
	{
		p = my_starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * my_setenv - Initialize a new environment variable,
 *              or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *         constant function prototype.
 * Return: Always 0
 */
int my_setenv(my_info_t *info)
{
	if (info->my_argc != 3)
	{
		_eputs("Incorrect number of arguments\n");
		return (1);
	}
	if (_setenv(info, info->my_argv[1], info->my_argv[2]))
		return (0);
	return (1);
}

/**
 * my_unsetenv - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int my_unsetenv(my_info_t *info)
{
	int i;

	if (info->my_argc == 1)
	{
		_eputs("Too few arguments.\n");
		return (1);
	}
	for (i = 1; i <= info->my_argc; i++)
		_unsetenv(info, info->my_argv[i]);

	return (0);
}

/**
 * my_populate_env_list - populates env linked list
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int my_populate_env_list(my_info_t *info)
{
	my_list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		my_add_node_end(&node, environ[i], 0);
	info->my_env = node;
	return (0);
}

