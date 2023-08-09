#include "pmalloc.h"


void* prealloc(in out void* ptr, in size_t size) {
	return prealloc_aligned(ptr, size, 0);
}
