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
#include <limits.h>
#include <dirent.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>

// Define useful macros
#define STR(S) #S
#define ERR(...) \
dprintf(2, "\e[31m < EZMEM %s:%d in %s() : Error : ", __FILE__, __LINE__, __FUNCTION__); \
dprintf(2, __VA_ARGS__ ); dprintf(2, " > \e[0m\n" );

#define FAT_ERR(...) ERR(__VA_ARGS__); \
dprintf(2, "\e[31;1m < EZMEM FATAL ERROR : EXIT >\e[0m\n" ); exit(1);


// Define some internal constants
#define MAIN_FOLDER "./.ezmem"
#define MEM_FOLDER "./.ezmem/mem/"
#define LOG_FILE "./.ezmem/log.memlog"
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
	void	*ptr;
	size_t	siz;
	t_location	loc;
}	t_memblk;

//////////////////////////////////////////////////////////// srcs/utils_io.h
static inline size_t str_len( const char* str )
{
	size_t	len;

	len = 0;
	while (str && str[len])
		len++;
	return ( len );
}


static inline void put_chr( int fd, char c )
{
	write( fd, &c, 1 );
}

void	put_str( int fd, char* str )
{
	write( fd, str, str_len( str ) );
}

//////////////////////////////////////////////////////////// srcs/utils_file.h
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

static inline int create_file(char* path, void (*func)(int fd))
{
	int fd = 0;
	struct stat st = { 0 };

	if (stat(path, &st) == -1)
	{
		fd = open(path, O_CREAT | O_RDWR | O_CLOEXEC, 0700);
	}
	if (fd < 1)
	{
		// TODO: error
	}
	if (func)
	{
		func(fd);
	}

	close(fd);
}

//////////////////////////////////////////////////////////// srcs/utils_nbr.h
static inline size_t put_nbr( int fd, ssize_t num )
{
	ssize_t	ret;

	ret = 0;
	if (num >= 10)
	{
		ret += put_nbr( fd, num / 10 );
		ret += put_nbr( fd, num % 10 );
	}
	if (num < 10)
	{
		put_chr( fd, '0' + num );
		ret += 1;
	}
	return ( ret );
}

static inline size_t put_nbr_base( int fd, ssize_t num, ssize_t base, char* b_chars )
{
	ssize_t	ret;

	ret = 0;
	if (num >= base)
	{
		ret += put_nbr_base( fd, num / base, base, b_chars );
		ret += put_nbr_base( fd, num % base, base, b_chars );
	}
	if (num < base)
	{
		put_chr( fd, b_chars[num] );
		ret += 1;
	}
	return ( ret );
}

//////////////////////////////////////////////////////////// srcs/id_management.h
static inline int get_curr_id( size_t* num_ptr )
{
	size_t		number = 0;
	char		input[ID_MAX_LEN] = { [0 ... ( ID_MAX_LEN - 1 )] = 0 }; // Use designated initializers to zero the array
	int			fd = -1;
	int			ret = 0;

	fd = open( IDS_FILE, O_RDONLY | O_CLOEXEC );
	if (fd < 0)
		return ( 1 );

	ret = read( fd, &input, ID_MAX_LEN ); // read from file into buffer
	if (ret < 0)
		return ( 1 );

	input[ret] = '\0'; // NULL terminate input buffer

	number = strtoull( input, NULL, 10 );
	*num_ptr = number;
	close( fd );
	return( 0 );
}

static inline int update_id( size_t curr_id )
{
	int			fd = -1;
	int			ret = 0;

	fd = open( IDS_FILE, O_RDWR | O_TRUNC, 0700 );
	if (fd < 0)
		return ( 1 );

	put_nbr( fd, curr_id + 1 );

	close( fd );
	return( 0 );
}

//////////////////////////////////////////////////////////// srcs/constructor.h
static inline void	constructor() __attribute__( ( constructor ) );

static inline void writ_readme( int fd )
{
	static char str[] = "\
## Content of `.ezmem` folder\n\
- README : this short explanation\n\
- .ids.memid : internal file to track current ID whitout the use of global variable\n\
- log.memlog : Log of all the calls to `malloc` and `free`\n\
- mem / : contains the memory blocks\n\
- leaks / : contains the memory blocks that have never been `free`d; A leaked block also contains a memory dump to help you find the source of your leak\n\
\n\
At the end of execution a `report.memreport` is generated to summarize the last recorded memory state.\n\
";

	put_str( fd, str );
}

static inline void writ_init_id( int fd )
{
	put_str( fd, "0" );
}

static inline int get_memid( long long* num_ptr );

static inline void create_mem_report( int sig );

static inline void	constructor()
{
	struct stat st = { 0 };
	if (stat( MAIN_FOLDER, &st ) != -1)
	{
		system( "rm -rf ./" MAIN_FOLDER );
	}

	create_dir( MAIN_FOLDER );
	create_dir( MEM_FOLDER );
	create_dir( LEAKS_FOLDER );

	create_file( LOG_FILE, NULL );
	create_file( IDS_FILE, writ_init_id );
	create_file( README_FILE, writ_readme );

	signal( SIGINT, create_mem_report );
	signal( SIGQUIT, create_mem_report );
}

//////////////////////////////////////////////////////////// srcs/destructor.h
static inline void	destructor() __attribute__( ( destructor ) );

static inline void create_mem_report( int sig )
{
	printf( "MEM REPORT CALLED\n" );
	signal( sig, SIG_DFL );
	kill( 0, sig );
}

static inline void	destructor()
{
	// code here
	create_mem_report( 0 );
}

//////////////////////////////////////////////////////////// srcs/output_data.h
typedef enum e_allo_or_free
{
	ALLO,
	FREE
}	t_aof;

static inline int parse_id_siz( t_memblk *mem, size_t *id, char *s )
{
	const uintptr_t mem_ptr = ( uintptr_t ) mem->ptr;
	uintptr_t ptr = 0;
	size_t i = 0;

	if (s && s[0] == '.' || !s)
	{
		return ( 0 );
	}
	while (s && s[i] && s[i] != 'A')
		i++;

	ptr = strtoull( s + i + 2, NULL, 16 );
	if (ptr == mem_ptr)
	{
		i = 0;
		// Parse ID
		*id = strtoull( s + 2, NULL, 10 );
		// Parse SIZ
		while (s && s[i] && s[i] != 'S')
			i++;
		mem->siz = strtoull( s + i + 2, NULL, 10 );

		return( 1 );
	}
	return ( 0 );
}

static inline int detect_id( t_memblk *mem, t_aof aof, size_t *id )
{
	// ID management
	//	- GET ID
	if (aof == ALLO)
	{
		if (get_curr_id( id ))
		{
			ERR( "get_curr_id: *id %p", id );
			return ( 1 );
		}
		//	- INCREMENT ID
		if (update_id( *id ))
		{
			ERR( "update_id: *id %p", id );
			return ( 2 );
		}
	}
	else if (aof == FREE)
	{
		// read dir and match mem->ptr and get ID and SIZ of mem->ptr
		DIR *ffd;
		struct dirent *ent;
		ffd = opendir( MEM_FOLDER );
		if (ffd == NULL)
		{
			ERR( "opendir: cannot open folder %s | ffd : %p", MEM_FOLDER, ffd );
			return ( 3 );
		}
		ent = ( struct dirent* ) 1;
		while (ent)
		{
			ent = readdir( ffd );
			if (ent && parse_id_siz( mem, id, ent->d_name ))
			{
				break;
			}
		}
		closedir( ffd );
	}
	return ( 0 );
}

static inline void output_data( t_memblk *mem, t_aof aof )
{
	int		fd = -1;
	size_t	id = 0;
	int		ret = 0;

	ret = detect_id( mem, aof, &id );
	if (ret)
	{
		ERR( "detect_id : RET %d", ret )
	}
	if (mem->siz == 0) // size of ZERO = UNKNOWN = ignore
	{
		return;
	}

	// Generate filename
	char fname[FNAME_MAXLEN] = { [0 ... ( FNAME_MAXLEN - 1 )] = 0 };
	snprintf( fname, FNAME_MAXLEN, MEM_FOLDER "I_%ld__S_%ld__A_%#llX", id, mem->siz, ( uintptr_t ) mem->ptr );

	// Open file
	if (aof == ALLO)
	{
		fd = open( fname, O_CREAT | O_WRONLY | O_TRUNC, 0700 );
	}
	else if (aof == FREE)
	{
		fd = open( fname, O_WRONLY | O_APPEND );
	}
	if (fd < 0)
	{
		ERR( "open memblk [%s] file in output_data()", fname );
	}
	if (aof == ALLO)
	{
		// Write DATA to file
		//	- ID
		put_nbr( fd, id );
		put_str( fd, " - ID\n" );
		//	- SIZ
		put_nbr( fd, mem->siz );
		put_str( fd, " - SIZE\n" );
		//	- ADDR
		put_str( fd, "0x" );
		put_nbr_base( fd, ( unsigned long long ) mem->ptr, 16, "0123456789ABCDEF" );
		put_str( fd, " - ADDR\n" );
		// one empty line
		put_str( fd, "\n" );
	}
	//	- LOCATION
	char loc[LOC_MAXLEN];
	if (aof == ALLO)
	{
		snprintf( loc, LOC_MAXLEN, "ALLO : %s:%ld in %s()\n", mem->loc.file, mem->loc.line, mem->loc.func );
	}
	else if (aof == FREE)
	{
		snprintf( loc, LOC_MAXLEN, "FREE : %s:%ld in %s()\n", mem->loc.file, mem->loc.line, mem->loc.func );
	}
	put_str( fd, loc );
	close( fd );

	if (aof == FREE)
	{
		char frename[FNAME_MAXLEN] = { [0 ... ( FNAME_MAXLEN - 1 )] = 0 };
		snprintf( frename, FNAME_MAXLEN, "%s__R", fname );
		if (rename( fname, frename ))
		{
			ERR( "rename memblk [%s] >> [%s] file in output_data()", fname, frename );
		}
	}

	int summ_fd;

	summ_fd = open( LOG_FILE, O_WRONLY | O_APPEND );
	if (summ_fd < 0)
	{
		ERR( "open summary [%s] file in output_data()", LOG_FILE );
	}
	dprintf( summ_fd, "%s : ID %-16ld - SIZE %-16ld - ADDR %#X | %s", ( aof == ALLO ) ? ( "ALLO" ) : ( "FREE" ), id, mem->siz, mem->ptr, loc );
	close( summ_fd );
}

//////////////////////////////////////////////////////////// srcs/wrap_allo_free.h
static inline void *_WRAPPED_malloc( size_t size, size_t line, const char *func, const char *file )
{
	t_memblk	mem = ( t_memblk ){ NULL, size, ( t_location ) { line, func, file } };
	void *ptr = NULL;

	mem.ptr = malloc( size ); // Call real malloc

	output_data( &mem, ALLO );

	return ( mem.ptr );
}

static inline void	_WRAPPED_free( void *ptr, int line, const char *func, const char *file )
{
	// code here
	t_memblk	mem = ( t_memblk ){ ptr, 0, ( t_location ) { line, func, file } };

	output_data( &mem, FREE );

	free( ptr );
}

# define malloc(x) _WRAPPED_malloc(x, __LINE__, __FUNCTION__, __FILE__)
# define free(x) _WRAPPED_free(x, __LINE__, __FUNCTION__, __FILE__)

#endif //EZMEM
