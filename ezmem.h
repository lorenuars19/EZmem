#ifndef EZMEM
 #define EZMEM

//srcs/init.h
#include "stdio.h"

#define FOLDER .ezmem

#define MEM_PATH FOLDER/mem
#define SUMMARY_PATH FOLDER/summary
#define LEAKS_PATH FOLDER/leaks

//srcs/wrap.h
# define malloc(x) _WRAPPED_malloc(x, __LINE__, __FUNCTION__, __FILE__)
# define free(x) _WRAPPED_free(x, __LINE__, __FUNCTION__, __FILE__)

#endif //EZMEM
