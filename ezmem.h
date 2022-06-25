#ifndef EZMEM
 #define EZMEM

//////////////////////////////////////////////////////////// srcs/init.h
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
#define IDS_FILE ".ezmem/.ids.memid"
#define README_FILE ".ezmem/README.txt"

//////////////////////////////////////////////////////////// srcs/utils.h
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
	close(fd);
}

static inline void put_chr_fd(int fd, char c)
{
	write(fd, &c, 1);
}

static inline ssize_t put_nbr_fd(int fd, ssize_t num)
{
	ssize_t	ret;

	ret = 0;
	if (num >= 10)
	{
		ret += put_nbr_fd(fd, num / 10);
		ret += put_nbr_fd(fd, num % 10);
	}
	if (num < 10)
	{
		put_chr_fd(fd, '0' + num);
		ret += 1;
	}
	return (ret);
}

static inline ssize_t put_nbr_base_fd(int fd, ssize_t num, ssize_t base, char* b_chars)
{
	ssize_t	ret;

	ret = 0;
	if (num >= base)
	{
		ret += put_nbr_base(num / base, base, b_chars);
		ret += put_nbr_base(num % base, base, b_chars);
	}
	if (num < base)
	{
		put_chr_fd(fd, b_chars[num]);
		ret += 1;
	}
	return (ret);
}

//////////////////////////////////////////////////////////// srcs/constructor.h
static inline void	constructor() __attribute__((constructor));

static inline void	constructor()
{

	create_dir(MAIN_FOLDER);
	create_dir(MEM_FOLDER);
	create_dir(LEAKS_FOLDER);

	create_file(SUMMARY_FILE);
	create_file(IDS_FILE);
	create_file(README_FILE);


}

//////////////////////////////////////////////////////////// srcs/destructor.h
static inline void	destructor() __attribute__((destructor));

static inline void	destructor()
{
	// code here
}

//////////////////////////////////////////////////////////// srcs/wrap.h
static inline void* _WRAPPED_malloc(size_t size, int line, const char* func, const char* file)
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
