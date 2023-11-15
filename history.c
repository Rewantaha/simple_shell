#include "shell.h"

/**
 * my_get_history_file - gets the history file
 * @my_info: parameter struct
 *
 * Return: allocated string containg history file
 */
char *my_get_history_file(info_t *my_info)
{
	char *my_buf, *dir;

	dir = my_getenv(my_info, "HOME=");
	if (!dir)
		return (NULL);
	my_buf = malloc(sizeof(char) * (my_strlen(dir) + my_strlen(HIST_FILE) + 2));
	if (!my_buf)
		return (NULL);
	my_buf[0] = 0;
	my_strcpy(my_buf, dir);
	my_strcat(my_buf, "/");
	my_strcat(my_buf, HIST_FILE);
	return (my_buf);
}

/**
 * my_write_history - creates a file, or appends to an existing file
 * @my_info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int my_write_history(info_t *my_info)
{
	ssize_t fd;
	char *filename = my_get_history_file(my_info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = my_info->my_history; node; node = node->next)
	{
		my_putsfd(node->str, fd);
		my_putfd('\n', fd);
	}
	my_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * my_read_history - reads history from file
 * @my_info: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int my_read_history(info_t *my_info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *my_buf = NULL, *filename = my_get_history_file(my_info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	my_buf = malloc(sizeof(char) * (fsize + 1));
	if (!my_buf)
		return (0);
	rdlen = read(fd, my_buf, fsize);
	my_buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(my_buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (my_buf[i] == '\n')
		{
			my_buf[i] = 0;
			my_build_history_list(my_info, my_buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		my_build_history_list(my_info, my_buf + last, linecount++);
	free(my_buf);
	my_info->my_histcount = linecount;
	while (my_info->my_histcount-- >= HIST_MAX)
		my_delete_node_at_index(&(my_info->my_history), 0);
	my_renumber_history(my_info);
	return (my_info->my_histcount);
}

/**
 * my_build_history_list - adds entry to a history linked list
 * @my_info: Structure containing potential arguments. Used to maintain
 * @my_buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int my_build_history_list(info_t *my_info, char *my_buf, int linecount)
{
	list_t *node = NULL;

	if (my_info->my_history)
		node = my_info->my_history;
	my_add_node_end(&node, my_buf, linecount);

	if (!my_info->my_history)
		my_info->my_history = node;
	return (0);
}

/**
 * my_renumber_history - renumbers the history linked list after changes
 * @my_info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int my_renumber_history(info_t *my_info)
{
	list_t *node = my_info->my_history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (my_info->my_histcount = i);
}

