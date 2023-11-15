#include "shell.h"

/**
 * my_strncpy - copies a string
 * @my_dest: the destination string to be copied to
 * @my_src: the source string
 * @n: the amount of characters to be copied
 * Return: the concatenated string
 */
char *my_strncpy(char *my_dest, char *my_src, int n)
{
	int i, j;
	char *s = my_dest;

	i = 0;
	while (my_src[i] != '\0' && i < n - 1)
	{
		my_dest[i] = my_src[i];
		i++;
	}
	if (i < n)
	{
		j = i;
		while (j < n)
		{
			my_dest[j] = '\0';
			j++;
		}
	}
	return (s);
}

/**
 * my_strncat - concatenates two strings
 * @my_dest: the first string
 * @my_src: the second string
 * @n: the amount of bytes to be maximally used
 * Return: the concatenated string
 */
char *my_strncat(char *my_dest, char *my_src, int n)
{
	int i, j;
	char *s = my_dest;

	i = 0;
	j = 0;
	while (my_dest[i] != '\0')
		i++;
	while (my_src[j] != '\0' && j < n)
	{
		my_dest[i] = my_src[j];
		i++;
		j++;
	}
	if (j < n)
		my_dest[i] = '\0';
	return (s);
}

/**
 * my_strchr - locates a character in a string
 * @my_s: the string to be parsed
 * @my_c: the character to look for
 * Return: (my_s) a pointer to the memory area my_s
 */
char *my_strchr(char *my_s, char my_c)
{
	do {
		if (*my_s == my_c)
			return (my_s);
	} while (*my_s++ != '\0');

	return (NULL);
}

