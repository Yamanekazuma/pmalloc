#include "pmalloc.h"

void* pcalloc(in size_t size) {
	return pcalloc_pcalloc(size, 0);
}
