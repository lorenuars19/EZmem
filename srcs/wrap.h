static inline void* _WRAPPED_malloc(size_t size, int line, const char* func, const char* file)
{
	// code here
	return (NULL);
}

static inline void	_WRAPPED_free(void* ptr, int line, const char* func, const char* file)
{
	// code here
}

// # define malloc(x) _WRAPPED_malloc(x, __LINE__, __FUNCTION__, __FILE__)
// # define free(x) _WRAPPED_free(x, __LINE__, __FUNCTION__, __FILE__)
