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
			return ( 1 );
		}
		//	- INCREMENT ID
		if (update_id( *id ))
		{
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
		dprintf( 2, "\e[31;1m < EZMEM : Error : detect_id : RET %d > \e[0m\n", ret );
		exit( ret );
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
		//TODO:error
		dprintf( 2, "\e[31;1m < EZMEM : Error : open memblk [%s] file in output_data()\n", fname );
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
			dprintf( 2, "\e[31;1m < EZMEM : Error : rename memblk [%s] >> [%s] file in output_data()\n", fname, frename );
		}
	}

	int summ_fd;

	summ_fd = open( LOG_FILE, O_WRONLY | O_APPEND );
	if (summ_fd < 0)
	{
		dprintf( 2, "\e[31;1m < EZMEM : Error : open summary [%s] file in output_data()\n", LOG_FILE );
	}
	dprintf( summ_fd, "%s : ID %-16ld - SIZE %-16ld - ADDR %#X | %s", ( aof == ALLO ) ? ( "ALLO" ) : ( "FREE" ), id, mem->siz, mem->ptr, loc );

	close( summ_fd );


}
