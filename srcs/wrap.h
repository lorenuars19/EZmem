typedef enum e_allo_or_free
{
	ALLO,
	FREE
}	t_aof;

#define FNAME_MAXLEN 1024
#define LOC_MAXLEN 512

static inline int parse_id_siz( t_memblk *mem, size_t *id, char *s )
{
	const uintptr_t mem_ptr = ( uintptr_t ) mem->ptr;
	uintptr_t ptr = 0;
	size_t i = 0;

	if (s && s[0] == '.')
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

#define D printf( " < MEM ptr %p siz %ld | LOC %s:%d in %s() > \n",	\
	mem->ptr, mem->siz, mem->loc.file, mem->loc.line, mem->loc.func );

static inline void output_data( t_memblk *mem, t_aof aof )
{
	int		fd = -1;
	size_t	id = 0;

	// ID management
	//	- GET ID
	if (aof == ALLO)
	{
		if (get_curr_id( &id ))
		{
			// TODO: error
			puts( "Error : curr_id" );
		}
		printf( " < id %d > \n", id );
		//	- INCREMENT ID
		if (update_id( id ))
		{
			//TODO: error
			puts( "Error : update_id" );
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
			//TODO: error
			puts( "Error: FREE get ID failed to open dir" MEM_FOLDER );
		}
		ent = ( struct dirent* ) 1;
		while (ent)
		{
			ent = readdir( ffd );
			if (ent && parse_id_siz( mem, &id, ent->d_name ))
			{
				break;
			}
		}
		closedir( ffd );
	}
	if (mem->siz == 0)
	{
		puts( "Error : siz ZERO" );
		return;
	}
	// Generate filename
	char fname[FNAME_MAXLEN] = { [0 ... ( FNAME_MAXLEN - 1 )] = 0 };
	snprintf( fname, FNAME_MAXLEN, MEM_FOLDER "I_%08ld__S_%08ld__A_%#llX", id, mem->siz, ( uintptr_t ) mem->ptr );

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
		//TODO:error
		printf( "Error: open memblk [%s] file output_data\n", fname );
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
}


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
