#include "shell.h"

/**
 * my_exit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: exits with a given exit status
 *         (0) if info->my_argv[0] != "exit"
 */
int my_exit(my_info_t *info)
{
	int my_exitcheck;

	if (info->my_argv[1])  /* If there is an exit argument */
	{
	my_exitcheck = my_erratoi(info->my_argv[1]);
	if (my_exitcheck == -1)
	{
	info->my_status = 2;
	print_error(info, "Illegal number: ");
	_eputs(info->my_argv[1]);
	_eputchar('\n');
	return (1);
	}
	info->my_err_num = my_erratoi(info->my_argv[1]);
	return (-2);
	}
	info->my_err_num = -1;
	return (-2);
}

/**
 * my_cd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Always 0
 */
int my_cd(my_info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
	_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->my_argv[1])
	{
	dir = my_getenv(info, "HOME=");
	if (!dir)
	chdir_ret = /* TODO: what should this be? */
	chdir((dir = my_getenv(info, "PWD=")) ? dir : "/");
	else
	chdir_ret = chdir(dir);
	}
	else if (my_strcmp(info->my_argv[1], "-") == 0)
	{
	if (!my_getenv(info, "OLDPWD="))
	{
		_puts(s);
	_putchar('\n');
	return (1);
	}
	_puts(my_getenv(info, "OLDPWD=")), _putchar('\n');
	chdir_ret = /* TODO: what should this be? */
	chdir((dir = my_getenv(info, "OLDPWD=")) ? dir : "/");
	}
	else
	chdir_ret = chdir(info->my_argv[1]);
	if (chdir_ret == -1)
	{
	print_error(info, "can't cd to ");
	_eputs(info->my_argv[1]), _eputchar('\n');
	}
	else
	{
	my_setenv(info, "OLDPWD", my_getenv(info, "PWD="));
	my_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * my_help - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Always 0
 */
int my_help(my_info_t *info)
{
	char **my_arg_array;

	my_arg_array = info->my_argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
	_puts(*my_arg_array); /* temp att_unused workaround */
	return (0);
}

