#include "shell.h"

/**
 * my_get_environ - returns the string array copy of our environ
 * @my_info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **my_get_environ(info_t *my_info)
{
	if (!my_info->environ || my_info->env_changed)
	{
		my_info->environ = list_to_strings(my_info->env);
		my_info->env_changed = 0;
	}

	return (my_info->environ);
}

/**
 * my_unsetenv - Remove an environment variable
 * @my_info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: 1 on delete, 0 otherwise
 * @my_var: the string env var property
 */
int my_unsetenv(info_t *my_info, char *my_var)
{
	list_t *my_node = my_info->env;
	size_t my_i = 0;
	char *my_p;

	if (!my_node || !my_var)
		return (0);

	while (my_node)
	{
		my_p = starts_with(my_node->str, my_var);
		if (my_p && *my_p == '=')
		{
			my_info->env_changed = delete_node_at_index(&(my_info->env), my_i);
			my_i = 0;
			my_node = my_info->env;
			continue;
		}
		my_node = my_node->next;
		my_i++;
	}
	return (my_info->env_changed);
}

/**
 * my_setenv - Initialize a new environment variable,
 *             or modify an existing one
 * @my_info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @my_var: the string env var property
 * @my_value: the string env var value
 * Return: Always 0
 */
int my_setenv(info_t *my_info, char *my_var, char *my_value)
{
	char *my_buf = NULL;
	list_t *my_node;
	char *my_p;

	if (!my_var || !my_value)
		return (0);

	my_buf = malloc(_strlen(my_var) + _strlen(my_value) + 2);
	if (!my_buf)
		return (1);
	_strcpy(my_buf, my_var);
	_strcat(my_buf, "=");
	_strcat(my_buf, my_value);
	my_node = my_info->env;
	while (my_node)
	{
		my_p = starts_with(my_node->str, my_var);
		if (my_p && *my_p == '=')
		{
			free(my_node->str);
			my_node->str = my_buf;
			my_info->env_changed = 1;
			return (0);
		}
		my_node = my_node->next;
	}
	add_node_end(&(my_info->env), my_buf, 0);
	free(my_buf);
	my_info->env_changed = 1;
	return (0);
}

