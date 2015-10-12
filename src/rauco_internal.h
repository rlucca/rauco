#pragma once

#include <stdlib.h>
#include <errno.h> // see /usr/include/sys/errno.h

#include "rauco.h"


struct internal_handler {
	const netcaller_setup_t *setup;
	int *fds; // last is -1
	unsigned int checksum;
	size_t buffer_size;
	char *buffer;
};


/**
	\brief Calculate the checksum of `ih'.

	\param [in] ih, internal handler reference
	\return Checksum of the `ih' passed.
*/
unsigned int ih_calculate_checksum(struct internal_handler *ih);

/**
	\brief Validate that `ih' is not null and the checksum is correct.

	\param [in] ih, internal handler reference
	\return 0, success. 1, error.
*/
int ih_checksum_valid(struct internal_handler *ih);

/**
	\brief Push the `fd' in `fds' and create a last element with value '-1'.

	\param [in] ih, internal handler reference
	\param [in] fd, file descriptor/socket
	\return 0, success. 1, error.
*/
int ih_add_fd(struct internal_handler *ih, int fd);
