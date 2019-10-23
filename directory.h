#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_

#include <stdio.h>	
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char** search_files(const char* path, const char* format, size_t* num);
size_t count_files(const char* path);

void free_filenames(char** filenames, size_t* num);	
#endif
