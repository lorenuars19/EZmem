static inline void	constructor() __attribute__((constructor));

static inline void create_dir(char* path)
{
	int ret = 0;
	struct stat st = { 0 };

	if (stat(path, &st) == -1)
	{
		ret = mkdir(path, 0700);
	}
	if (ret)
	{
		//TODO: error
	}
}

static inline int create_file(char* path)
{
	int fd = 0;
	struct stat st = { 0 };

	if (stat(path, &st) == -1)
	{
		fd = open(path, O_CREAT | O_RDWR, 0700);
	}
	if (fd < 1)
	{
		// TODO: error
	}
}

static inline void	constructor()
{

	create_dir(MAIN_FOLDER);
	create_dir(MEM_FOLDER);
	create_dir(LEAKS_FOLDER);

	create_file(SUMMARY_FILE);
	create_file(IDS_FILE);
	create_file(README_FILE);


}
