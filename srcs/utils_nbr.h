static inline size_t put_nbr( int fd, ssize_t num )
{
	ssize_t	ret;

	ret = 0;
	if (num >= 10)
	{
		ret += put_nbr( fd, num / 10 );
		ret += put_nbr( fd, num % 10 );
	}
	if (num < 10)
	{
		put_chr( fd, '0' + num );
		ret += 1;
	}
	return ( ret );
}

static inline size_t put_nbr_base( int fd, ssize_t num, ssize_t base, char* b_chars )
{
	ssize_t	ret;

	ret = 0;
	if (num >= base)
	{
		ret += put_nbr_base( fd, num / base, base, b_chars );
		ret += put_nbr_base( fd, num % base, base, b_chars );
	}
	if (num < base)
	{
		put_chr( fd, b_chars[num] );
		ret += 1;
	}
	return ( ret );
}
