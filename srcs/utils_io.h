static inline size_t str_len( const char* str )
{
	size_t	len;

	len = 0;
	while (str && str[len])
		len++;
	return ( len );
}


static inline void put_chr( int fd, char c )
{
	write( fd, &c, 1 );
}

void	put_str( int fd, char* str )
{
	write( fd, str, str_len( str ) );
}
