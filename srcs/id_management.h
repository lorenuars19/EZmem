static inline int get_curr_id(size_t* num_ptr)
{
	size_t		number = 0;
	char		input[ID_MAX_LEN] = { [0 ... (ID_MAX_LEN - 1)] = 0 }; // Use designated initializers to zero the array
	int			fd = -1;
	int			ret = 0;

	fd = open(IDS_FILE, O_RDONLY | O_CLOEXEC);
	FD(fd);

	if (fd < 0)
		return (1);

	ret = read(fd, &input, ID_MAX_LEN); // read from file into buffer
	if (ret < 0)
		return (1);

	input[ret] = '\0'; // NULL terminate input buffer

	number = strtoull(input, NULL, 10);
	*num_ptr = number;
	close(fd);
	return(0);
}

static inline int update_id(size_t curr_id)
{
	int			fd = -1;

	fd = open(IDS_FILE, O_RDWR | O_TRUNC, 0700);
	FD(fd);

	if (fd < 0)
		return (1);

	put_nbr(fd, curr_id + 1);

	close(fd);
	return(0);
}
