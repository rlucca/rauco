#include "rauco_internal.h"

unsigned int ih_calculate_checksum(struct internal_handler *ih)
{
	// It'll crash when ih is null
	size_t aux = (size_t) ih ^ (size_t) ih->setup;
	return aux ^ (size_t) ih->fds ^ (size_t) ih->buffer_size;
}

int ih_checksum_valid(struct internal_handler *ih)
{
	if (ih && ih_calculate_checksum(ih) == ih->checksum)
		return 0;

	return 1;
}

static int ih_get_fd(struct internal_handler *ih, int fd, unsigned *n)
{
	int *fds;

	*n = 0;
	for (fds = ih->fds; *fds != -1; fds++, (*n)++)
	{
		if (*fds == fd)
			return fd;
	}

	(*n)++;
	return -1;
}

int ih_add_fd(struct internal_handler *ih, int fd)
{
	int *fds;
	unsigned n;

	if (ih_get_fd(ih, fd, &n) != -1)
	{
		errno = EALREADY;
		return 1;
	}

	fds = realloc(ih->fds, sizeof(int) * (n + 1));
	if (fds)
	{
		if (fds != ih->fds)
		{
			ih->fds = fds;
			ih->checksum = ih_calculate_checksum(ih);
		}

		fds[n - 1] = fd;
		fds[n] = -1;
		return 0;
	}

	errno = ENOMEM;
	return -1;
}
