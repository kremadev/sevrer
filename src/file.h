#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>

void file_set_root(const char *root);
char *get_file_content(char *path, int *size);

#endif
