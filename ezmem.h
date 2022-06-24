#ifndef EZMEM
 #define EZMEM

/* === srcs/init.h === */
// Include external functions
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
// Define useful macros
#define STR(S) #S

// Define some internal constants
#define MAIN_FOLDER ".ezmem"
#define MEM_FOLDER ".ezmem/mem"
#define SUMMARY_FILE ".ezmem/summary.memlog"
#define LEAKS_FOLDER ".ezmem/leaks"
#define IDS_FILE ".ezmem/ids.memid"
#define README_FILE ".ezmem/README.txt"

/* === srcs/constructor.h === */static inline void	constructor() __attribute__((constructor));

static inline void create_dir(char* path)
{
	int ret = 0;
	struct stat st = { 0 };

	if (stat(path, &st) == -1)
	{
		ret = mkdir(path, 0700);
	}
	if (ret)
	{
		//TODO: error
	}
}

static inline int create_file(char* path)
{
	int fd = 0;
	struct stat st = { 0 };

	if (stat(path, &st) == -1)
	{
		fd = open(path, O_CREAT | O_RDWR, 0700);
	}
	if (fd < 1)
	{
		// TODO: error
	}
}

static inline void	constructor()
{

	create_dir(MAIN_FOLDER);
	create_dir(MEM_FOLDER);
	create_dir(LEAKS_FOLDER);

	create_file(SUMMARY_FILE);
	create_file(IDS_FILE);
	create_file(README_FILE);


}

/* === srcs/destructor.h === */static inline void	destructor() __attribute__((destructor));

static inline void	destructor()
{
	// code here
}

/* === srcs/wrap.h === */static inline void* _WRAPPED_malloc(size_t size, int line, const char* func, const char* file)
{
	// code here
	return (NULL);
}

static inline void	_WRAPPED_free(void* ptr, int line, const char* func, const char* file)
{
	// code here
}

// # define malloc(x) _WRAPPED_malloc(x, __LINE__, __FUNCTION__, __FILE__)
// # define free(x) _WRAPPED_free(x, __LINE__, __FUNCTION__, __FILE__)

#endif //EZMEM
