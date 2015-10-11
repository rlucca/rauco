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
