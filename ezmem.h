#ifndef EZMEM
 #define EZMEM

// = = = = = = = = = = = = = =
//  srcs/init.h              =
// = = = = = = = = = = = = = =

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
#define MEM_PATH MAIN_FOLDER/mem
#define SUMMARY_PATH MAIN_FOLDER/summary
#define LEAKS_PATH MAIN_FOLDER/leaks
#define IDS_PATH MAIN_FOLDER/ids

// = = = = = = = = = = = = = =
//  srcs/constructor.h       =
// = = = = = = = = = = = = = =

static inline void	constructor() __attribute__((constructor));

static inline void	constructor()
{
	mkdir(MAIN_FOLDER, 0555);
}

// = = = = = = = = = = = = = =
//  srcs/destructor.h        =
// = = = = = = = = = = = = = =

static inline void	destructor() __attribute__((destructor));

static inline void	destructor()
{
	// code here
}

// = = = = = = = = = = = = = =
//  srcs/wrap.h              =
// = = = = = = = = = = = = = =




static inline void* _WRAPPED_malloc(size_t size, int line, const char* func, const char* file)
{
	// code here
}

static inline void	_WRAPPED_free(void* ptr, int line, const char* func, const char* file)
{
	// code here
}

# define malloc(x) _WRAPPED_malloc(x, __LINE__, __FUNCTION__, __FILE__)
# define free(x) _WRAPPED_free(x, __LINE__, __FUNCTION__, __FILE__)

#endif //EZMEM
