#include "shell.h"

/**
 * my_hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int my_hsh(info_t *info, char **av)
{
	ssize_t r = 0;
	int my_builtin_ret = 0;

	while (r != -1 && my_builtin_ret != -2)
	{
		my_clear_info(info);
		if (my_interactive(info))
			my_puts("$ ");
		my_putchar(BUF_FLUSH);
		r = my_get_input(info);
		if (r != -1)
		{
			my_set_info(info, av);
			my_builtin_ret = my_find_builtin(info);
			if (my_builtin_ret == -1)
				my_find_cmd(info);
		}
		else if (my_interactive(info))
			my_putchar('\n');
		my_free_info(info, 0);
	}
	my_write_history(info);
	my_free_info(info, 1);
	if (!my_interactive(info) && info->status)
		exit(info->status);
	if (my_builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (my_builtin_ret);
}

/**
 * my_find_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int my_find_builtin(info_t *info)
{
	int i, my_built_in_ret = -1;
	my_builtin_table my_builtintbl[] = {
		{"exit", my_myexit},
		{"env", my_myenv},
		{"help", my_myhelp},
		{"history", my_myhistory},
		{"setenv", my_mysetenv},
		{"unsetenv", my_myunsetenv},
		{"cd", my_mycd},
		{"alias", my_myalias},
		{NULL, NULL}
	};

	for (i = 0; my_builtintbl[i].type; i++)
		if (my_strcmp(info->argv[0], my_builtintbl[i].type) == 0)
		{
			info->line_count++;
			my_built_in_ret = my_builtintbl[i].func(info);
			break;
		}
	return (my_built_in_ret);
}

/**
 * my_find_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void my_find_cmd(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!my_is_delim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = my_find_path(info, my_getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		my_fork_cmd(info);
	}
	else
	{
		if ((my_interactive(info) || my_getenv(info, "PATH=")
			|| info->argv[0][0] == '/') && my_is_cmd(info, info->argv[0]))
			my_fork_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			my_print_error(info, "not found\n");
		}
	}
}

/**
 * my_fork_cmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void my_fork_cmd(info_t *info)
{
	pid_t my_child_pid;

	my_child_pid = fork();
	if (my_child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (my_child_pid == 0)
	{
		if (execve(info->path, info->argv, my_get_environ(info)) == -1)
		{
			my_free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				my_print_error(info, "Permission denied\n");
		}
	}
}

