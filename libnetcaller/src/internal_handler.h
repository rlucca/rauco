/**
	\file internal_handler.h
	\brief Implements the interface to do the work
*/
#pragma once

#include "libnetcaller.h"
#include <stdio.h>
#include <errno.h>

// max buffer of 8kb
#define MAX_BUF_LEN 8192
#define MINIMAL_DATA_TO_PEEK 1

int handle_receive(const Netcaller *, int);
const Netcaller *find_entry(const Netcaller *, const char *buffer);
