#include "pmalloc.h"
#include "arena.h"


#define SIZE_ALIGNMENT 8
#define SIZE_ALIGNMENT_MASK (SIZE_ALIGNMENT - 1)
#define GET_ALIGNED_SIZE(n) ((n + SIZE_ALIGNMENT_MASK) & ~SIZE_ALIGNMENT_MASK)


static inline _pmalloc_chunk* _pmalloc_create_chunk(_pmalloc_state* arena, size_t size);
static inline _pmalloc_chunk* _pmalloc_create_bin_chunk(_pmalloc_state* arena, size_t size, size_t binIdx);
static inline _pmalloc_chunk* _pmalloc_create_mmap_chunk(_pmalloc_state* arena, size_t size);


void* pmalloc(in size_t size) {
	if (size == 0) {
		return NULL;
	}

	if (!_pmalloc_initialized) {
		_pmalloc_initialize();
	}

	if (size < _PM_MIN_SIZE) {
		size = _PM_MIN_SIZE;
	}
	else {
		size = GET_ALIGNED_SIZE(size);
	}

	_pmalloc_state* arena = _pmalloc_get_arena();
	if (arena == NULL) {
		return NULL;
	}

	if (_pmalloc_is_mmap_size(size)) {
		_pmalloc_chunk* chunk = _pmalloc_create_mmap_chunk(arena, size);
		return _pmalloc_chunk2ptr(chunk);
	}
	else {
		size_t binIdx = _pmalloc_size2binidx(size);
		size_t count = arena->counts[binIdx];
		if (count > 0) {
			_pmalloc_chunk* chunk = _pmalloc_pop_bin(arena, binIdx);
			return _pmalloc_chunk2ptr(chunk);
		}
		else {
			_pmalloc_chunk* chunk = _pmalloc_create_bin_chunk(arena, size, binIdx);
			return _pmalloc_chunk2ptr(chunk);
		}
	}
}


static inline _pmalloc_chunk* _pmalloc_create_mmap_chunk(_pmalloc_state* arena, size_t size) {
	_pmalloc_chunk* chunk = _pmalloc_create_chunk(arena, size);
	chunk->arena = arena;

	assert(_pmalloc_chunk2binidx(chunk) == 0);
	return chunk;
}

static inline _pmalloc_chunk* _pmalloc_create_bin_chunk(_pmalloc_state* arena, size_t size, size_t binIdx) {
	assert(binIdx == _pmalloc_size2binidx(size));

	_pmalloc_chunk* chunk = _pmalloc_create_chunk(arena, size);
	chunk->arena = arena;
	_pmalloc_chunk_set_size(chunk, binIdx);

	assert(_pmalloc_chunk2binidx(chunk) == binIdx);
	return chunk;
}

static inline _pmalloc_chunk* _pmalloc_create_chunk(_pmalloc_state* arena, size_t size) {
	_pmalloc_chunk* chunk = HeapAlloc(arena->heap, HEAP_NO_SERIALIZE, size + sizeof(_pmalloc_state*));
	return chunk;
}
