#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char *shortv;
	char *longv;
	char **dest;
	int flag;
} arg_t;

void args_add_argument(char *shortv, char *longv, char **dest, int takes_value);
void args_check_arguments(int argc, char **argv);

#endif
