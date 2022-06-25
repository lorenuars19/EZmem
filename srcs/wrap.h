typedef enum e_allo_or_free
{
	ALLO,
	FREE
}	t_aof;

static inline void output_data( t_memblk *mem, t_aof aof )
{
	int		fd = -1;
	size_t	id = 0;

	printf( " < MEM ptr %p siz %ld | LOC %s:%d in %s() > \n",
			mem->ptr, mem->siz, mem->loc.file, mem->loc.line, mem->loc.func );

	// ID management
	//	- GET ID
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

	// Open file
	// Write DATA to file
	//	- ID
	//	- SIZ
	//	- LOCATION
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
