static inline void quit( int sig )
{
	dprintf( 2, "\n\e[32;1m < Done creating memory report %s > \e[0m\n", REPORT_FILE );
	signal( sig, SIG_DFL );
	kill( 0, sig );
}

static inline int process_fname( char *s, t_mstat *mstat )
{
	size_t	i = 0;
	size_t	slen = 0;
	char	freed = 0;

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





	return ( 0 );
}

#define ANIM_MAXLEN 3
#define ANIM_FRAMES 4

static inline void create_mem_report( int sig )
{
	t_mstat mstat = ( t_mstat ){ 0, 0, 0, 0 };
	size_t n_files;
	static char anim[ANIM_FRAMES][ANIM_MAXLEN] = { "-", "/", "-" , "\\" };
	struct stat st = { 0 };

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
	dprintf( 2, "\n\e[32;1m < Creating memory report %s> \e[0m", anim[0] );
	while (ent)
	{
		dprintf( 2, "\e[32;1m < Creating memory report %s >\e[0m", anim[( n_files / 256 ) % ANIM_FRAMES] );
		ent = readdir( ffd );
		if (ent && process_fname( ent->d_name, &mstat ))
		{
			ERR( "process_fname : non-zero return | ent %p", ent );
		}
		n_files++;
	}
	closedir( ffd );


	quit( sig );
}
