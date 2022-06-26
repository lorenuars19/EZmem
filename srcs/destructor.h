static inline void	destructor() __attribute__( ( destructor ) );

static inline void create_mem_report( void )
{

}

static inline void	destructor()
{
	// code here
	create_mem_report();
}
