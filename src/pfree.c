#include "pmalloc.h"
#include "arena.h"

void pfree(in void* ptr) {
	if (ptr == NULL) {
		return;
	}

	_pmalloc_chunk* chunk = _pmalloc_ptr2chunk(ptr);
	size_t binIdx = _pmalloc_chunk2binidx(chunk);
	_pmalloc_state* arena = _pmalloc_chunk2arena(chunk);
	size_t count = arena-> counts[binIdx];
	if (binIdx == 0 || count >= _PM_FILL_COUNT) {
		HeapFree(arena->heap, HEAP_NO_SERIALIZE, chunk);
	}
	else {
		_pmalloc_push_bin(arena, binIdx, _pmalloc_chunk2ptr(chunk));
	}
}
