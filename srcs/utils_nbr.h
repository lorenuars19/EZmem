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

static inline int	is_wsp( char c )
{
	if (c == '\t' || c == '\v' || c == '\f' || c == '\r' || c == ' ')
		return ( 1 );
	return ( 0 );
}

static inline ssize_t str_to_nbr( char* s )
{
	ssize_t	num;
	int		sign;

	num = 0;
	sign = 1;
	while (s && *s && is_wsp( *s ))
		s++;
	while (s && *s >= '0' && *s <= '9')
		num = ( num * 10 ) + ( *s++ - '0' );
	if (num > LONG_MAX)
	{
		if (sign == 1)
			return ( -1 );
		else
			return ( 0 );
	}
	if (sign == 1)
		return ( num );
	else
		return ( -num );
	return ( 0 );
}

static inline ssize_t str_to_nbr_base( char* s, int base )
{
	ssize_t	num;
	int		sign;

	num = 0;
	sign = 1;
	while (s && *s && is_wsp( *s ))
		s++;
	while (s && *s >= '0' && *s <= '9')
		num = ( num * base ) + ( *s++ - '0' );
	if (num > LONG_MAX)
	{
		if (sign == 1)
			return ( -1 );
		else
			return ( 0 );
	}
	if (sign == 1)
		return ( num );
	else
		return ( -num );
	return ( 0 );
}
