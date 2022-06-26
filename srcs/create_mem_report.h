static inline void quit( int sig )
{
	dprintf( 2, "\e[2K\e[0G\e[32;1m < EZMEM : Done creating memory report %s > \e[0m\n\n", REPORT_FILE );

	system( "echo ; cat " REPORT_FILE );

	kill( 0, sig );
}

static inline void dump_leak( int fd, t_memblk *mem )
{
	size_t i = 0;

	dprintf( fd, "\nLEAK DUMP : ID %ld SIZ %ld ADDR %#llX\n", mem->id, mem->siz, mem->ptr );
	unsigned long long *cast = ( unsigned long long * ) mem->ptr;

	dprintf( fd, "\n\nHEX :\n", mem->id, mem->siz, mem->ptr );
	i = 0;
	while (i < mem->siz)
	{
		dprintf( fd, "%#16llX ", cast[i] );
		i++;
	}

	dprintf( fd, "\n\nDEC :\n", mem->id, mem->siz, mem->ptr );
	i = 0;
	while (i < mem->siz)
	{
		dprintf( fd, "%16lld ", cast[i] );
		i++;
	}

	dprintf( fd, "\n\nSTR RAW :\n", mem->id, mem->siz, mem->ptr );
	i = 0;
	while (i < mem->siz)
	{
		put_chr( fd, '\'' );
		if (cast[i] < ' ')
		{
			dprintf( fd, "\\%16lld ", cast[i] );
		}
		else
		{
			put_chr( fd, cast[i] );
		}
		put_chr( fd, '\'' );
		put_chr( fd, ' ' );
		i++;
	}

	dprintf( fd, "\n\nSTR :\n", mem->id, mem->siz, mem->ptr );
	i = 0;
	while (i < mem->siz)
	{
		put_chr( fd, cast[i] );
		i++;
	}
}


static inline int process_fname( char *s, t_mstat * mstat )
{
	size_t		i = 0;
	size_t		slen = 0;
	char		freed = 0;
	t_memblk	mem = ( t_memblk ){ 0, NULL, 0, ( t_location ) { 0, NULL, NULL } };

	if (s && s[0] == '.' || !s)
	{
		return ( 0 );
	}

	slen = str_len( s );
	if (s[slen - 1] == 'R')
	{
		freed = 1;
	}
	i = 0;
	// Parse ID
	mem.id = strtoull( s + 2, NULL, 10 );
	// Parse SIZ
	while (s && s[i] && s[i] != 'S')
		i++;
	mem.siz = strtoull( s + i + 2, NULL, 10 );
	// Parse ADDR
	while (s && s[i] && s[i] != 'A')
		i++;
	mem.ptr = ( void * ) strtoull( s + i + 2, NULL, 16 );

	// printf( " < process_fname : mem id %ld siz %ld, ptr %p> \n", mem.id, mem.siz, mem.ptr );

	mstat->total_mem_use += mem.siz;
	mstat->allo_cnt++;

	if (freed == 1)
	{
		mstat->total_mem_free += mem.siz;
		mstat->free_cnt++;
		return ( 0 );
	}

	char fname[FNAME_MAXLEN] = { [0 ... ( FNAME_MAXLEN - 1 )] = 0 };
	snprintf( fname, FNAME_MAXLEN, MEM_FOLDER MEM_FMT, mem.id, mem.siz, ( uintptr_t ) mem.ptr );
	int fd = open( fname, O_RDONLY );
	if (fd < 0)
	{
		ERR( "open : invalid fd %d", fd );
	}

	char buffer[REPORT_BUFFER_MAXLEN] = { [0 ... ( REPORT_BUFFER_MAXLEN - 1 )] = 0 };

	int ret = 1;
	ret = read( fd, buffer, REPORT_BUFFER_MAXLEN );
	if (ret < 0)
	{
		ERR( "read: ret %d", ret );
	}
	buffer[ret] = '\0';
	close( fd );

	char leak_fname[FNAME_MAXLEN] = { [0 ... ( FNAME_MAXLEN - 1 )] = 0 };
	snprintf( leak_fname, FNAME_MAXLEN, LEAKS_FOLDER "ID_%ld__SIZ_%ld__ADDR_%#llX_LEAKED", mem.id, mem.siz, ( uintptr_t ) mem.ptr );
	int leak_fd = open( leak_fname, O_CREAT | O_WRONLY | O_TRUNC, 0700 );
	if (leak_fd < 0)
	{
		ERR( "open : invalid leak_fd %d", leak_fd );
	}
	put_str( leak_fd, buffer );
	dump_leak( leak_fd, &mem );
	close( leak_fd );

	int report_fd = open( REPORT_FILE, O_CREAT | O_WRONLY | O_APPEND, 0700 );
	dprintf( report_fd, "LEAK : ID %-16lld SIZ %-16lld ADDR %#llX\n", mem.id, mem.siz, ( uintptr_t ) mem.ptr );
	close( report_fd );
	return ( 0 );
}

#define ANIM_MAXLEN 3
#define ANIM_FRAMES 4

static inline void create_mem_report( int sig )
{
	t_mstat mstat = ( t_mstat ){ 0, 0, 0, 0 };
	size_t n_files;
	static char anim[ANIM_FRAMES][ANIM_MAXLEN] = { "|", "/", "-" , "\\" };
	struct stat st = { 0 };

	signal( sig, SIG_DFL );
	if (stat( REPORT_FILE, &st ) != -1)
	{
		ERR( "%s already exists", REPORT_FILE );
	}

	DIR *ffd;
	ffd = opendir( MEM_FOLDER );
	if (ffd == NULL)
	{
		ERR( "opendir : cannot open folder %s | ffd : %p", MEM_FOLDER, ffd );
		quit( sig );
	}
	struct dirent *ent = ( struct dirent* ) 1;
	n_files = 0;
	dprintf( 2, "\n\e[32;1m < EZMEM : Creating memory report %s> \e[0m", anim[0] );
	int init_report_fd = open( REPORT_FILE, O_CREAT | O_WRONLY | O_APPEND, 0700 );
	dprintf( init_report_fd, "\
================================================================================\n\
=== MEMORY REPORT\n\n" );
	close( init_report_fd );
	while (ent)
	{
		dprintf( 2, "\e[2K\e[0G\e[32;1m < EZMEM : Creating memory report %s >\e[0m", anim[( n_files / 8 ) % ANIM_FRAMES] );
		usleep( 512 * 10 );
		ent = readdir( ffd );
		if (ent && process_fname( ent->d_name, &mstat ))
		{
			ERR( "process_fname : non-zero return | ent %p", ent );
		}
		n_files++;
	}
	closedir( ffd );

	int report_fd = open( REPORT_FILE, O_CREAT | O_WRONLY | O_APPEND, 0700 );
	dprintf( report_fd, "\n\
================================================================================\n\
MEMORY STATS : \n\
- total memory used  : % lld\n\
- total memory freed : % lld\n\
- alloc count        : % lld\n\
- free count         : % lld\n", mstat.total_mem_use, mstat.total_mem_free, mstat.allo_cnt, mstat.free_cnt );
	close( report_fd );

	quit( sig );
}
