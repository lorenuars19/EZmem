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
