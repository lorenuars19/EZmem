static inline void	constructor() __attribute__((constructor));

#define MK_DIR(X) if (stat(X, &st) == -1) {mkdir( X , 0700);}

static inline void	constructor()
{
	struct stat st = { 0 };

	MK_DIR(MAIN_FOLDER);
	MK_DIR(MEM_PATH);
	MK_DIR(SUMMARY_PATH);
	MK_DIR(LEAKS_PATH);
	MK_DIR(IDS_PATH);

}
