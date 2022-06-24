
static inline void	constructor() __attribute__((constructor));

static inline void	constructor()
{
	mkdir(FOLDER);
}
