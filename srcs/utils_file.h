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
	close(fd);
}

static inline void put_chr_fd(int fd, char c)
{
	write(fd, &c, 1);
}

static inline ssize_t put_nbr_fd(int fd, ssize_t num)
{
	ssize_t	ret;

	ret = 0;
	if (num >= 10)
	{
		ret += put_nbr_fd(fd, num / 10);
		ret += put_nbr_fd(fd, num % 10);
	}
	if (num < 10)
	{
		put_chr_fd(fd, '0' + num);
		ret += 1;
	}
	return (ret);
}

static inline ssize_t put_nbr_base_fd(int fd, ssize_t num, ssize_t base, char* b_chars)
{
	ssize_t	ret;

	ret = 0;
	if (num >= base)
	{
		ret += put_nbr_base(num / base, base, b_chars);
		ret += put_nbr_base(num % base, base, b_chars);
	}
	if (num < base)
	{
		put_chr_fd(fd, b_chars[num]);
		ret += 1;
	}
	return (ret);
}
