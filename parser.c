#include "shell.h"

/**
 * my_is_cmd - determines if a file is an executable command
 * @my_info: the info struct
 * @my_path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int my_is_cmd(my_info_t *my_info, char *my_path)
{
	struct stat st;

	(void)my_info;
	if (!my_path || stat(my_path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * my_dup_chars - duplicates characters
 * @path_str: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to new buffer
 */
char *my_dup_chars(char *path_str, int start, int stop)
{
	static char buf[1024];
	int i = 0, k = 0;

	for (k = 0, i = start; i < stop; i++)
		if (path_str[i] != ':')
			buf[k++] = path_str[i];
	buf[k] = 0;
	return (buf);
}

/**
 * my_find_path - finds this cmd in the PATH string
 * @my_info: the info struct
 * @path_str: the PATH string
 * @cmd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *my_find_path(my_info_t *my_info, char *path_str, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!path_str)
		return (NULL);
	if ((my_strlen(cmd) > 2) && my_starts_with(cmd, "./"))
	{
		if (my_is_cmd(my_info, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!path_str[i] || path_str[i] == ':')
		{
			path = my_dup_chars(path_str, curr_pos, i);
			if (!*path)
				my_strcat(path, cmd);
			else
			{
				my_strcat(path, "/");
				my_strcat(path, cmd);
			}
			if (my_is_cmd(my_info, path))
				return (path);
			if (!path_str[i])
				break;
			curr_pos = i;
		}
		i++;
	}
	return (NULL);
}

