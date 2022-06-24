static inline void	constructor() __attribute__((constructor));

static inline void	constructor()
{

	create_dir(MAIN_FOLDER);
	create_dir(MEM_FOLDER);
	create_dir(LEAKS_FOLDER);

	create_file(SUMMARY_FILE);
	create_file(IDS_FILE);
	create_file(README_FILE);


}
