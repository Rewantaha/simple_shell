#include "shell.h"

/**
 * my_input_buf - buffers chained commands
 * @my_info: parameter struct
 * @my_buf: address of buffer
 * @my_len: address of len var
 *
 * Return: bytes read
 */
ssize_t my_input_buf(info_t *my_info, char **my_buf, size_t *my_len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*my_len) /* if nothing left in the buffer, fill it */
	{
		/*bfree((void **)my_info->my_cmd_buf);*/
		free(*my_buf);
		*my_buf = NULL;
		signal(SIGINT, my_sigintHandler);
#if USE_GETLINE
		r = getline(my_buf, &len_p, stdin);
#else
		r = _getline(my_info, my_buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*my_buf)[r - 1] == '\n')
			{
				(*my_buf)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			my_info->my_linecount_flag = 1;
			remove_comments(*my_buf);
			build_history_list(my_info, *my_buf, my_info->my_histcount++);
			/* if (_strchr(*my_buf, ';')) is this a command chain? */
			{
				*my_len = r;
				my_info->my_cmd_buf = my_buf;
			}
		}
	}
	return (r);
}

/**
 * my_get_input - gets a line minus the newline
 * @my_info: parameter struct
 *
 * Return: bytes read
 */
ssize_t my_get_input(info_t *my_info)
{
	static char *my_buf; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(my_info->my_arg), *p;

	_putchar(BUF_FLUSH);
	r = my_input_buf(my_info, &my_buf, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len)	/* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buf position */
		p = my_buf + i; /* get pointer for return */

		check_chain(my_info, my_buf, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (is_chain(my_info, my_buf, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			my_info->my_cmd_buf_type = CMD_NORM;
		}

		*buf_p = p; /* pass back pointer to current command position */
		return (_strlen(p)); /* return length of current command */
	}

	*buf_p = my_buf; /* else not a chain, pass back buffer from _getline() */
	return (r); /* return length of buffer from _getline() */
}

/**
 * my_read_buf - reads a buffer
 * @my_info: parameter struct
 * @my_buf: buffer
 * @my_i: size
 *
 * Return: r
 */
ssize_t my_read_buf(info_t *my_info, char *my_buf, size_t *my_i)
{
	ssize_t r = 0;

	if (*my_i)
		return (0);
	r = read(my_info->my_readfd, my_buf, READ_BUF_SIZE);
	if (r >= 0)
		*my_i = r;
	return (r);
}

/**
 * my__getline - gets the next line of input from STDIN
 * @my_info: parameter struct
 * @my_ptr: address of pointer to buffer, preallocated or NULL
 * @my_length: size of preallocated my_ptr buffer if not NULL
 *
 * Return: s
 */
int my__getline(info_t *my_info, char **my_ptr, size_t *my_length)
{
	static char my_buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *my_ptr;
	if (p && my_length)
		s = *my_length;
	if (i == len)
		i = len = 0;

	r = my_read_buf(my_info, my_buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchr(my_buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - my_buf) : len;
	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(new_p, my_buf + i, k - i);
	else
		_strncpy(new_p, my_buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (my_length)
		*my_length = s;
	*my_ptr = p;
	return (s);
}

/**
 * my_sigintHandler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void my_sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}

