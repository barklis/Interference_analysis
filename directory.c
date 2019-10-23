#include "directory.h"

char** search_files(const char* path, const char* format, size_t* num)
{
	struct dirent* entry;
	DIR* dp;

	size_t size = count_files(path);

	
	dp = opendir(path);
	
	if(NULL == dp)
	{
		perror("Can't open directory!\n");
		return NULL;
	}

	
	char** files = (char**) malloc(sizeof(char*) * size);
	size_t matches = 0;

	const char delim[2] = ".";
	char* token;

	while(entry = readdir(dp))
	{
		token = strtok(entry->d_name, delim);
		token = strtok(NULL, delim);
		
		if( (token != NULL) && (strlen(token) > 2) && (!strncmp(token, format, 3)) )
		{
			int chars = strlen(entry->d_name);
			files[matches] = (char*) malloc(sizeof(char) * (chars + 1));
			strncpy(files[matches], entry->d_name, chars + 1);
			matches++;
		}
	}

	closedir(dp);
	
	*num = matches;
	return files;
}

size_t count_files(const char* path)
{
	size_t size = 0;
	struct dirent* entry;
	DIR* dp;
	dp = opendir(path);
	
	if(NULL == dp)
	{
		perror("Can't open directory!\n");
		return 0;
	}

	while(entry = readdir(dp))
		size++;

	closedir(dp);
	
	return size;
}

void free_filenames(char** filenames, size_t* num)
{
	size_t i = 0;
	for(i = 0; i < *num; i++)
	{
		free(filenames[i]);
	}

	free(filenames); 
}


