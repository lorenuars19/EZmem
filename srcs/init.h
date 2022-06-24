
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
