/**
	\file rauco.h
	\brief Types and functions which can be used externally to the library.
*/
#pragma once

#include <stdio.h>

/**
	\brief Buffer data. This data is freed after the `handler' function.
*/
typedef struct {
	size_t length;				 /**< length of buffer */
	const unsigned char *buffer; /**< buffer data. It's not zero-terminated */
} netcaller_buffer_t;


/**
	\brief Definition of a callback that the library can call.

	This struct is the program's core. First, the `id' is the first
	byte of the buffer. It's used to identify which handler should be
	called. After the identification, the total size of the package is
	calculate sum the fixed and extra up. After the size validation,
	the handler function informed is called with buffer. This buffer is
	all package including the identificator. When the handler returns,
	the buffer is destroyed.
*/
typedef struct {
	/** Package identificator. First byte of the buffer. */
	unsigned char id;
	/** Inform the size of the package statically. It can be zero. */
	size_t fixed_size;
	/** Inform the size of the package based on the buffer. It can be NULL. */
	size_t (*extra_size)(netcaller_buffer_t *);
	/** How to interpret the buffer data to the current program? */
	void (*handler)(netcaller_buffer_t *);
} netcaller_setup_t;


/**
	\brief Create a netcaller object that need be passed to other functions.

	\param [in] Pointer to configuration of the object.
				The pointer is holded by the library, so it can be freed.

	\return A value not NULL that need by passed to others function.
*/
void *netcaller_create(const netcaller_setup_t *);

/**
	\brief Insert `fd' to be observed for reading.

	\param [in] netcaller object reference.
	\param [in] fd, socket to be observed for reading.

	\return 0, success. -1 and errno in case of error.
*/
int netcaller_register(void *, int);

/**
	\brief Remove `fd' from the observation's list.

	\param [in] netcaller object reference.
	\param [in] fd, socket to be removed

	\return 0, success. -1 and errno in case of error.
*/
int netcaller_deregister(void *, int);

/**
	\brief Verify all sockets registered and do the handler of the data.

	\param [in] netcaller object reference.

	\return 0, success. -1 and errno in case of error.
*/
int netcaller_dispatch(void *);

/**
	\brief Destroy the netcaller object passed.

	\param [in] Object to destroy.
*/
void netcaller_destroy(void *);
