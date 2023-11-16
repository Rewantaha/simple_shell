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

#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

#define CMD_NORM    0
#define CMD_OR      1
#define CMD_AND     2
#define CMD_CHAIN   3

#define CONVERT_LOWERCASE   1
#define CONVERT_UNSIGNED    2

#define USE_GETLINE 0
#define USE_STRTOK  0

#define HIST_FILE   ".simple_shell_history"
#define HIST_MAX    4096

extern char **environ;

typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

typedef struct my_passinfo
{
	char *my_arg;
	char **my_argv;
	char *my_path;
	int my_argc;
	unsigned int my_line_count;
	int my_err_num;
	int my_linecount_flag;
	char *my_fname;
	list_t *my_env;
	list_t *my_history;
	list_t *my_alias;
	char **my_environ;
	int my_env_changed;
	int my_status;
	char **my_cmd_buf;
	int my_cmd_buf_type;
	int my_readfd;
	int my_histcount;
} my_info_t;

#define MY_INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
0, 0, 0, 0, NULL, 0, 0}

typedef struct builtin
{
	char *my_type;
	int (*my_func)(my_info_t *);
} my_builtin_table;

/* Function prototypes here */

#endif

