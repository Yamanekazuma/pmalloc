#include "pmalloc.h"
#include "arena.h"
#include <string.h>


#define SIZE_ALIGNMENT 8
#define SIZE_ALIGNMENT_MASK (SIZE_ALIGNMENT - 1)
#define GET_ALIGNED_SIZE(n) ((n + SIZE_ALIGNMENT_MASK) & ~SIZE_ALIGNMENT_MASK)


static inline void* _prealloc_create_chunkptr(_pmalloc_state* arena, size_t size, _pmalloc_chunk* chunk);
static inline _pmalloc_chunk* _prealloc_create_chunk(_pmalloc_state* arena, size_t size, _pmalloc_chunk* chunk);


void* prealloc(in out void* ptr, in size_t size) {
	if (size == 0) {
		pfree(ptr);
		return NULL;
	}

	if (ptr == NULL) {
		return pmalloc(size);
	}

	if (!_pmalloc_initialized) {
		_pmalloc_initialize();
	}

	_pmalloc_chunk* old_chunk = _pmalloc_ptr2chunk(ptr);
	_pmalloc_state* old_arena = _pmalloc_chunk2arena(old_chunk);
	size_t old_binIdx = _pmalloc_chunk2binidx(old_chunk);

	if (size < _PM_MIN_SIZE) {
		size = _PM_MIN_SIZE;
	}
	else {
		size = GET_ALIGNED_SIZE(size);
	}

	if (_pmalloc_is_mmap_binidx(old_binIdx)) {
		return _prealloc_create_chunkptr(old_arena, size, old_chunk);
	}
	else {
		size_t old_size = _pmalloc_binidx2size(old_binIdx);
		if (old_size > size) {
			// サイズ縮小時
			_pmalloc_chunk* chunk = old_chunk;
			size_t diff = old_size - size;
			size_t splitted_size = diff - sizeof(_pmalloc_state*);
			size_t splitted_binIdx = _pmalloc_size2binidx(splitted_size);
			size_t count = old_arena->counts[splitted_binIdx];
			if (diff >= _PM_MIN_SIZE + sizeof(_pmalloc_state*) && count < _PM_FILL_COUNT) {
				// 分割可能なら分割
				_pmalloc_chunk* splitted_chunk = (_pmalloc_chunk*)((uintptr_t)old_chunk + sizeof(_pmalloc_state*) + old_size);
				splitted_chunk->arena = old_chunk->arena;
				_pmalloc_chunk_set_size(splitted_chunk, splitted_binIdx);
				_pmalloc_push_bin(old_arena, splitted_binIdx, _pmalloc_chunk2ptr(splitted_chunk));
				return _pmalloc_chunk2ptr(chunk);
			}
			else {
				// 分割不可ならリサイズ
				return _prealloc_create_chunkptr(old_arena, size, old_chunk);
			}
		}
		else {
			// サイズ拡大時
			size_t count = old_arena->counts[old_binIdx];
			if (count < _PM_FILL_COUNT) {
				// binに空きがあるならbinに挿入して新規malloc
				void* ret = pmalloc(size);
				if (ret == NULL) {
					return NULL;
				}
				memcpy(ret, _pmalloc_chunk2ptr(old_chunk), old_size);
				_pmalloc_push_bin(old_arena, old_binIdx, _pmalloc_chunk2ptr(old_chunk));
				return ret;
			}
			else {
				// binに空きがないならリサイズ
				return _prealloc_create_chunkptr(old_arena, size, old_chunk);
			}
		}
	}
}

static inline void* _prealloc_create_chunkptr(_pmalloc_state* arena, size_t size, _pmalloc_chunk* chunk) {
	chunk = _prealloc_create_chunk(arena, size, chunk);
	if (chunk == NULL) {
		return NULL;
	}
	return _pmalloc_chunk2ptr(chunk);
}

static inline _pmalloc_chunk* _prealloc_create_chunk(_pmalloc_state* arena, size_t size, _pmalloc_chunk* chunk) {
	chunk = HeapReAlloc(arena->heap, HEAP_NO_SERIALIZE, chunk, size);
	if (chunk == NULL) {
		return NULL;
	}
	return chunk;
}
