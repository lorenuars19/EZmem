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
