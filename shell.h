#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM    0
#define CMD_OR      1
#define CMD_AND     2
#define CMD_CHAIN   3

/* for convert_number() */
#define CONVERT_LOWERCASE   1
#define CONVERT_UNSIGNED    2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE   ".simple_shell_history"
#define HIST_MAX    4096

extern char **environ;

/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
    int num;
    char *str;
    struct liststr *next;
} list_t;

/**
 * struct passinfo - contains pseudo-arguments to pass into a function,
 *                  allowing uniform prototype for function pointer struct
 * @arg: a string generated from getline containing arguments
 * @argv: an array of strings generated from arg
 * @path: a string path for the current command
 * @argc: the argument count
 * @line_count: the error count
 * @err_num: the error code for exit()s
 * @linecount_flag: if on count this line of input
 * @fname: the program filename
 * @env: linked list local copy of environ
 * @environ: custom modified copy of environ from LL env
 * @history: the history node
 * @alias: the alias node
 * @env_changed: on if environ was changed
 * @status: the return status of the last exec'd command
 * @cmd_buf: address of pointer to cmd_buf, on if chaining
 * @cmd_buf_type: CMD_type ||, &&, ;
 * @readfd: the fd from which to read line input
 * @histcount: the history line number count
 */
typedef struct passinfo
{
    char *arg;
    char **argv;
    char *path;
    int argc;
    unsigned int line_count;
    int err_num;
    int linecount_flag;
    char *fname;
    list_t *env;
    list_t *history;
    list_t *alias;
    char **environ;
    int env_changed;
    int status;

    char **cmd_buf; /* pointer to cmd ; chain buffer, for memory management */
    int cmd_buf_type; /* CMD_type ||, &&, ; */
    int readfd;
    int histcount;
} my_info_t;

#define MY_INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
    0, 0, 0}

/**
 * struct builtin - contains a builtin string and related function
 * @type: the builtin command flag
 * @func: the function
 */
typedef struct builtin
{
    char *type;
    int (*func)(my_info_t *);
} my_builtin_table;

/* toem_shloop.c */
int my_hsh(my_info_t *, char **);
int my_find_builtin(my_info_t *);
void my_find_cmd(my_info_t *);
void my_fork_cmd(my_info_t *);

/* toem_parser.c */
int my_is_cmd(my_info_t *, char *);
char *my_dup_chars(char *, int, int);
char *my_find_path(my_info_t *, char *, char *);

/* loophsh.c */
int my_loophsh(char **);

/* toem_errors.c */
void my_eputs(char *);
int my_eputchar(char);
int my_putfd(char c, int fd);
int my_putsfd(char *str, int fd);

/* toem_string.c */
int my_strlen(char *);
int my_strcmp(char *, char *);
char *my_starts_with(const char *, const char *);
char *my_strcat(char *, char *);

/* toem_string1.c */
char *my_strcpy(char *, char *);
char *my_strdup(const char *);
void my_puts(char *);
int my_putchar(char);

/* toem_exits.c */
char *my_strncpy(char *, char *, int);
char *my_strncat(char *, char *, int);
char *my_strchr(char *, char);

/* toem_tokenizer.c */
char **my_strtow(char *, char *);
char **my_strtow2(char *, char);

/* toem_realloc.c */
char *my_memset(char *, char, unsigned int);
void my_ffree(char **);
void *my_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int my_bfree(void **);

/* toem_atoi.c */
int my_interactive(my_info_t *);
int my_is_delim(char, char *);
int my_isalpha(int);
int my_atoi(char *);

/* toem_errors1.c */
int my_erratoi(char *);
void my_print_error(my_info_t *, char *);
int my_print_d(int, int);
char *my_convert_number(long int, int, int);
void my_remove_comments(char *);

/* toem_builtin.c */
int my_myexit(my_info_t *);
int my_mycd(my_info_t *);
int my_myhelp(my_info_t *);

/* toem_builtin1.c */
int my_myhistory(my_info_t *);
int my_myalias(my_info_t *);

/*toem_getline.c */
ssize_t my_get_input(my_info_t *);
int my_getline(my_info_t *, char **, size_t *);
void my_sigintHandler(int);

/* toem_getinfo.c */
void my_clear_info(my_info_t *);
void my_set_info(my_info_t *, char **);
void my_free_info(my_info_t *, int);

/* toem_environ.c */
char *my_getenv(my_info_t *, const char *);
int my_myenv(my_info_t *);
int my_mysetenv(my_info_t *);
int my_myunsetenv(my_info_t *);
int my_populate_env_list(my_info_t *);

/* toem_getenv.c */
char **my_get_environ(my_info_t *);
int my_unsetenv(my_info_t *, char *);
int my_setenv(my_info_t *, char *, char *);

/* toem_history.c */
char *my_get_history_file(my_info_t *my_info);
int my_write_history(my_info_t *my_info);
int my_read_history(my_info_t *my_info);
int my_build_history_list(my_info_t *my_info, char *buf, int linecount);
int my_renumber_history(my_info_t *my_info);

/* toem_lists.c */
list_t *my_add_node(list_t **, const char *, int);
list_t *my_add_node_end(list_t **, const char *, int);
size_t my_print_list_str(const list_t *);
int my_delete_node_at_index(list_t **, unsigned int);
void my_free_list(list_t **);

/* toem_lists1.c */
size_t my_list_len(const list_t *);
char **my_list_to_strings(list_t *);
size_t my_print_list(const list_t *);
list_t *my_node_starts_with(list_t *, char *, char);
ssize_t my_get_node_index(list_t *, list_t *);

/* toem_vars.c */
int my_is_chain(my_info_t *, char *, size_t *);
void my_check_chain(my_info_t *, char *, size_t *, size_t, size_t);
int my_replace_alias(my_info_t *);
int my_replace_vars(my_info_t *);
int my_replace_string(char **, char *);

#endif
