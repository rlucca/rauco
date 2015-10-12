#include "rauco_internal.h"


void *netcaller_create(const netcaller_setup_t *setup)
{
	struct internal_handler *ih;

	if (setup == NULL)
		return NULL;

	if ((ih = malloc(sizeof(struct internal_handler))) == NULL)
		return NULL;

	ih->fds = malloc(sizeof(int));

	if (ih->fds == NULL)
	{
		free(ih);
		ih = NULL;
		return NULL;
	}

	ih->setup = setup;
	ih->fds[0] = -1;
	ih->buffer_size = 0;
	ih->buffer = NULL;
	ih->checksum = ih_calculate_checksum(ih);

	return ih;
}

#if 0
int netcaller_register(void *h, int fd)
{
	//errno = ESRCH;
	return -1;
}

int netcaller_deregister(void *h, int fd)
{
	//errno = ESRCH;
	return -1;
}

int netcaller_dispatch(void *h)
{
	//errno = EBADF;
	return -1;
}
#endif

void netcaller_destroy(void *h)
{
	struct internal_handler *ih = (struct internal_handler *) h;

	if (ih_checksum_valid(ih))
		return ;

	ih->checksum = 0xDEAD;
	free(ih->buffer); ih->buffer = NULL;
	free(ih->fds); ih->fds = NULL;
	free(ih);
}
