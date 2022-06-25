// Include external functions
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <dirent.h>
#include <string.h>
#include <stdint.h>

// Define useful macros
#define STR(S) #S

// Define some internal constants
#define MAIN_FOLDER "./.ezmem"
#define MEM_FOLDER "./.ezmem/mem/"
#define SUMMARY_FILE "./.ezmem/summary.memlog"
#define LEAKS_FOLDER "./.ezmem/leaks"
#define IDS_FILE "./.ezmem/.ids.memid"
#define README_FILE "./.ezmem/README.txt"

// Define structs
typedef struct s_location
{
	size_t		line;
	const char	*func;
	const char	*file;
}	t_location;

typedef struct s_mem_blok
{
	void	*ptr;
	size_t	siz;
	t_location	loc;
}	t_memblk;
