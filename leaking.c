/*
**	Testing main for EZmem
*/

#include "ezmem.h"

#include <stdlib.h>
#include <time.h>

char		**tab_alloc( int x, int y )
{
	char	**tab;
	size_t	i;

	if (!( tab = ( char ** ) malloc( ( y + 1 ) * sizeof( char * ) ) ))
		return ( NULL );
	i = 0;
	while (i < y)
	{
		if (!( tab[i] = ( char * ) malloc( x * sizeof( char ) ) ))
			return ( NULL );
		i++;
	}
	tab[i] = NULL;
	return ( tab );
}

void		tab_free( char **tab )
{
	size_t	i;

	i = 0;
	while (tab && tab[i + 1])
	{
		if (rand() % 100 != 0)
		{
			free( tab[i] );
		}
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
	char	**tab;

	srand( time( 0 ) );
	tab = tab_alloc( 3, 300 );
	tab_free( tab );

	// while (1);
	// exit( 1 );

	return ( 0 );
}
