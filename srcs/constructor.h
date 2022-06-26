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
	signal( SIGTERM, create_mem_report );
	signal( SIGABRT, create_mem_report );
	signal( SIGBUS, create_mem_report );
	signal( SIGQUIT, create_mem_report );
}
