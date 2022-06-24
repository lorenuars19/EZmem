
static inline void	constructor() __attribute__((constructor));

static inline void	constructor()
{
	mkdir(MAIN_FOLDER, 0555);
}
