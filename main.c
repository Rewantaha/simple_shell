#include "shell.h"

/**
 * my_main - entry point
 * @my_ac: arg count
 * @my_av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int my_main(int my_ac, char **my_av)
{
	my_info_t my_info[] = { MY_INFO_INIT };
	int my_fd = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (my_fd)
		: "r" (my_fd));

	if (my_ac == 2)
	{
		my_fd = open(my_av[1], O_RDONLY);
		if (my_fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_my_eputs(my_av[0]);
				_my_eputs(": 0: Can't open ");
				_my_eputs(my_av[1]);
				_my_eputchar('\n');
				_my_eputchar(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		my_info->readfd = my_fd;
	}
	my_populate_env_list(my_info);
	my_read_history(my_info);
	my_hsh(my_info, my_av);
	return (EXIT_SUCCESS);
}

