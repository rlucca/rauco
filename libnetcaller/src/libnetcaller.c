/**
	\file libnetcaller.c
	\brief Implements the library's entry points
*/
#include "internal_handler.h"

int netcaller_generic_receive(const Netcaller *packets, int fd)
{
	if (packets == NULL || fd < 0)
	{
		errno = EINVAL;
		return -1;
	}
	return handle_receive(packets, fd);
}
