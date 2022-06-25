static inline void put_chr_fd(int fd, char c)
{
	write(fd, &c, 1);
}
