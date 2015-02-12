/**
	\file internal_handler.c
	\brief Implements the dirty work :)
*/
#include "internal_handler.h"
#include <sys/socket.h>
#include <limits.h>

static int check_minimum_data(const Netcaller *, const char *, int *);
static unsigned total_size(const Netcaller *, const char *);

int handle_receive(const Netcaller *packets, int fd)
{
	char buffer[MAX_BUF_LEN];
	int count;

	count = recv(fd, buffer, sizeof(buffer), MSG_PEEK);

	if (count >= MINIMAL_DATA_TO_PEEK)
	{
		const Netcaller *idx = find_entry(packets, buffer);

		if (idx == NULL)
		{
			errno = ENOENT;
			return -1;
		}

		if (!check_minimum_data(idx, buffer, &count))
		{
			// If it reach the minimum data,
			// it is safe to discard the data
			// and call the action with the buffer peeked
			(void) recv(fd, buffer, (size_t) count, 0);
			if (idx->callback) idx->callback(buffer);
			return 0;
		}
	}

	return (count == 0) ? 0 : -1;
}

const Netcaller *find_entry(const Netcaller *packets, const char *buffer)
{
	const Netcaller *current = packets;

	// Stops when reply's size isn't available
	while (current->fixed_size > 0 || current->calculate_extra_size != NULL)
	{
		if (current->id == *buffer)
			return current;
		current++;
	}

	// Element not found
	// If the lat element has a callback, it's necessary return it!
	if (current->callback)
		return current;

	return NULL;
}

static int check_minimum_data(
		const Netcaller *p,
		const char *buffer,
		int *buffer_len)
{
	unsigned int total = total_size(p, buffer);

	if (total <= INT_MAX && *buffer_len >= (int) total)
	{
		if (total > MINIMAL_DATA_TO_PEEK)
			*buffer_len = (int) total;
		return 0;
	}

	errno = EAGAIN;
	return -1;
}

static unsigned int total_size(const Netcaller *p, const char *buffer)
{
	unsigned int total = p->fixed_size;
	if (p->calculate_extra_size)
		total += p->calculate_extra_size(buffer);
	return total;
}
