/**
	\file libnetcaller.h
	\brief It is responsable to handle messages that arrives in a file descriptor.
*/
#pragma once

/**
	\brief Entry of a callback that the library can call.
*/
typedef struct
{
	char id;
	unsigned int fixed_size;
	unsigned int (*calculate_extra_size)(const char *);
	void (*callback)(const char *);
} Netcaller;


/**
	\brief Call the callback setup in Netcaller array.

	\param [in] netcaller, table which has the last element
				with size set to zero.
	\param [inout] fd, file descriptors of the socket that is
				receiving data from a client.

	\return 0, on sucess; -1, on error and sets the errno
*/
int netcaller_generic_receive(const Netcaller *, int);
