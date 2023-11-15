#include "shell.h"

/**
 * my_strtow - splits a string into words. Repeat delimiters are ignored
 * @my_str: the input string
 * @my_d: the delimiter string
 * Return: a pointer to an array of strings, or NULL on failure
 */

char **my_strtow(char *my_str, char *my_d)
{
    int i, j, k, m, numwords = 0;
    char **s;

    if (my_str == NULL || my_str[0] == 0)
        return (NULL);
    if (!my_d)
        my_d = " ";
    for (i = 0; my_str[i] != '\0'; i++)
        if (!my_is_delim(my_str[i], my_d) && (my_is_delim(my_str[i + 1], my_d) || !my_str[i + 1]))
            numwords++;

    if (numwords == 0)
        return (NULL);
    s = malloc((1 + numwords) * sizeof(char *));
    if (!s)
        return (NULL);
    for (i = 0, j = 0; j < numwords; j++)
    {
        while (my_is_delim(my_str[i], my_d))
            i++;
        k = 0;
        while (!my_is_delim(my_str[i + k], my_d) && my_str[i + k])
            k++;
        s[j] = malloc((k + 1) * sizeof(char));
        if (!s[j])
        {
            for (k = 0; k < j; k++)
                free(s[k]);
            free(s);
            return (NULL);
        }
        for (m = 0; m < k; m++)
            s[j][m] = my_str[i++];
        s[j][m] = 0;
    }
    s[j] = NULL;
    return (s);
}

/**
 * my_strtow2 - splits a string into words
 * @my_str: the input string
 * @my_d: the delimiter
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **my_strtow2(char *my_str, char my_d)
{
    int i, j, k, m, numwords = 0;
    char **s;

    if (my_str == NULL || my_str[0] == 0)
        return (NULL);
    for (i = 0; my_str[i] != '\0'; i++)
        if ((my_str[i] != my_d && my_str[i + 1] == my_d) ||
            (my_str[i] != my_d && !my_str[i + 1]) || my_str[i + 1] == my_d)
            numwords++;
    if (numwords == 0)
        return (NULL);
    s = malloc((1 + numwords) * sizeof(char *));
    if (!s)
        return (NULL);
    for (i = 0, j = 0; j < numwords; j++)
    {
        while (my_str[i] == my_d && my_str[i] != my_d)
            i++;
        k = 0;
        while (my_str[i + k] != my_d && my_str[i + k] && my_str[i + k] != my_d)
            k++;
        s[j] = malloc((k + 1) * sizeof(char));
        if (!s[j])
        {
            for (k = 0; k < j; k++)
                free(s[k]);
            free(s);
            return (NULL);
        }
        for (m = 0; m < k; m++)
            s[j][m] = my_str[i++];
        s[j][m] = 0;
    }
    s[j] = NULL;
    return (s);
}

