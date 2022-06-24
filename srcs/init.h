
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
