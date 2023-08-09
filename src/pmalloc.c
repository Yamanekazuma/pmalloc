#include "pmalloc.h"

void* pmalloc(in size_t size) {
	return pmalloc_aligned(size, 0);
}
