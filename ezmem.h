#ifndef EZMEM
 #define EZMEM

/* srcs/init.h
*/

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
#define MEM_PATH ".ezmem/mem"
#define SUMMARY_PATH ".ezmem/summary"
#define LEAKS_PATH ".ezmem/leaks"
#define IDS_PATH ".ezmem/ids"
#define README_PATH ".ezmem/README"

/* srcs/constructor.h
*/

static inline void	constructor() __attribute__((constructor));

#define MK_DIR(X) if (stat(X, &st) == -1) {mkdir( X , 0700);}

static inline void	constructor()
{
	struct stat st = { 0 };

	MK_DIR(MAIN_FOLDER);
	MK_DIR(MEM_PATH);
	MK_DIR(SUMMARY_PATH);
	MK_DIR(LEAKS_PATH);
	MK_DIR(IDS_PATH);

}

/* srcs/destructor.h
*/

static inline void	destructor() __attribute__((destructor));

static inline void	destructor()
{
	// code here
}

/* srcs/wrap.h
*/

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
