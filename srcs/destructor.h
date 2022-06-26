static inline void	destructor() __attribute__( ( destructor ) );

static inline void	destructor()
{
	create_mem_report( 0 );
}
