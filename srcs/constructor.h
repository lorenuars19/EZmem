static inline void	constructor() __attribute__( ( constructor ) );

static inline void writ_readme( int fd );
{
	static char str [] = "\
		Content of the EZMEM folder : \n\
			- mem/ :\n\
				Contains one file for each memory block\n\
			- leaks/ :\n\
				Contains one file per memory blocks that has never been freed\n\
			- summary.memlog :\n\
				Contains log of memory alloc / frees during execution\n\
			- .ids.memid :\n\
				Helper internal file tracking current ID\n\
		";

	put_str( str )
}

static inline void	constructor()
{

	create_dir( MAIN_FOLDER );
	create_dir( MEM_FOLDER );
	create_dir( LEAKS_FOLDER );

	create_file( SUMMARY_FILE, NULL );
	create_file( IDS_FILE, NULL );
	create_file( README_FILE, writ_readme );


}
