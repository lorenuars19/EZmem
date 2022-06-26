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
#include <signal.h>

// Define useful macros
#define STR(S) #S
#define ERR(...) \
dprintf(2, "\n\e[31m < EZMEM %s:%d in %s() : Error : ", __FILE__, __LINE__, __FUNCTION__); \
dprintf(2, __VA_ARGS__ ); dprintf(2, " > \e[0m\n" );

#define FAT_ERR(...) ERR(__VA_ARGS__); \
dprintf(2, "\e[31;1m < EZMEM FATAL ERROR : EXIT >\e[0m\n" ); exit(1);


// Define some internal constants
#define MAIN_FOLDER "./.ezmem"
#define MEM_FOLDER "./.ezmem/mem/"
#define LOG_FILE "./.ezmem/log.memlog"
#define REPORT_FILE "./.ezmem/report.memreport"
#define LEAKS_FOLDER "./.ezmem/leaks"
#define IDS_FILE "./.ezmem/.ids.memid"
#define README_FILE "./.ezmem/README.md"

#define FNAME_MAXLEN 1024
#define LOC_MAXLEN 512
#define ID_MAX_LEN 42

// Define structs
typedef struct s_location
{
	size_t		line;
	const char	*func;
	const char	*file;
}	t_location;

typedef struct s_mem_blok
{
	size_t	id;
	void	*ptr;
	size_t	siz;
	t_location	loc;
}	t_memblk;

typedef struct s_mem_stat
{
	size_t	total_mem_use;
	size_t	total_mem_free;
	size_t	allo_cnt;
	size_t	free_cnt;
}	t_mstat;
