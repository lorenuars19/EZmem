/*
**	Testing main for EZmem
*/

#include "ezmem.h"

#include <stdlib.h>

char** tab_alloc( size_t x, size_t y )
{
	char** tab;
	size_t	i;

	if (!( tab = ( char** ) malloc( ( y + 1 ) * sizeof( char* ) ) ))
		return ( NULL );
	i = 0;
	while (i < y)
	{
		tab[i] = ( char* ) malloc( x * sizeof( char ) );
		if (!( tab[i] ))
			return ( NULL );
		i++;
	}
	tab[i] = NULL;
	return ( tab );
}

void		tab_free( char** tab )
{
	size_t	i;

	i = 0;
	while (tab && tab[i])
	{
		free( tab[i] );
		tab[i] = NULL;
		i++;
	}
	if (tab)
	{
		free( tab );
		tab = NULL;
	}
}

int			main( void )
{
	char** tab;

	tab = tab_alloc( 3, 300 );
	tab_free( tab );
	return ( 0 );
}
