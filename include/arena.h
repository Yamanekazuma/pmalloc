#pragma once

#include "windows.h"
#include <assert.h>
#include <pmalloc/type.h>

struct __pmalloc_chunk;
struct __pmalloc_state;

typedef struct __pmalloc_chunk {
	struct __pmalloc_state* arena;	// 割り当て元arena, & 63でbinIdx取得, 0の場合はmmapped
	struct __pmalloc_chunk* fd;
} _pmalloc_chunk;

struct __pmalloc_chunkptr;
typedef struct __pmalloc_chunkptr {
	struct __pmalloc_chunkptr* fd;
} _pmalloc_chunkptr;

#define _PM_MAX_BINS 64
#define _PM_FILL_COUNT 7
#define _PM_MAX_BIN_SIZE 512
#define _PM_ARENA_ALIGNMENT 64
#define _PM_MIN_SIZE 16

typedef struct __pmalloc_state {
	CRITICAL_SECTION mutex;
	HANDLE heap;
	// bins[0]: UNUSED
	// bins[1]: 16bytes
	// bins[2]: 24bytes
	// ...
	// bins[63]: 512bytes
	_pmalloc_chunkptr bins[_PM_MAX_BINS];
	uint8_t counts[_PM_MAX_BINS];
	struct __pmalloc_state* next;
} _pmalloc_state;


static inline size_t _pmalloc_size2binidx(size_t size) {
	assert(size >= 16 && size <= _PM_MAX_BIN_SIZE && (size & 7) == 0);
	return (size >> 3) - 1;
}

static inline size_t _pmalloc_binidx2size(size_t binIdx) {
	assert(binIdx > 0 && binIdx < _PM_MAX_BINS);
	return (binIdx + 1) << 3;
}

static inline void _pmalloc_chunk_set_size(_pmalloc_chunk* chunk, size_t binIdx) {
	assert(binIdx > 0 && binIdx < _PM_MAX_BINS);
	chunk->arena = (_pmalloc_state*)((uintptr_t)(chunk->arena) | binIdx);
}

static inline _pmalloc_state* _pmalloc_chunk2arena(_pmalloc_chunk* chunk) {
	return (_pmalloc_state*)((uintptr_t)(chunk->arena) & ~(_PM_ARENA_ALIGNMENT - 1));
}

static inline size_t _pmalloc_chunk2binidx(_pmalloc_chunk* chunk) {
	return (size_t)((uintptr_t)(chunk->arena) & (_PM_ARENA_ALIGNMENT - 1));
}

static inline bool _pmalloc_is_mmap_size(size_t size) {
	return size > _PM_MAX_BIN_SIZE;
}

static inline bool _pmalloc_is_mmap_binidx(size_t binIdx) {
	return (binIdx == 0);
}

static inline _pmalloc_chunk* _pmalloc_ptr2chunk(void* chunkptr) {
	return (_pmalloc_chunk*)((uintptr_t)chunkptr - sizeof(_pmalloc_state*));
}

static inline void* _pmalloc_chunk2ptr(_pmalloc_chunk* chunk) {
	return (void*)&(chunk->fd);
}

static inline void _pmalloc_push_bin(_pmalloc_state* arena, size_t binIdx, _pmalloc_chunkptr* chunkptr) {
	uint8_t* pcount = &(arena->counts[binIdx]);
	_pmalloc_chunkptr* pbins = &(arena->bins[binIdx]);
	assert(*pcount < _PM_FILL_COUNT);

	++(*pcount);
	chunkptr->fd = pbins->fd;
	pbins->fd = (_pmalloc_chunkptr*)(&(chunkptr->fd));
}

static inline _pmalloc_chunk* _pmalloc_pop_bin(_pmalloc_state* arena, size_t binIdx) {
	uint8_t* pcount = &(arena->counts[binIdx]);
	_pmalloc_chunkptr* pbin = &(arena->bins[binIdx]);
	assert(*pcount > 0 && pbin->fd != NULL);

	--(*pcount);
	_pmalloc_chunkptr* bin = pbin->fd;
	pbin->fd = bin->fd;
	return _pmalloc_ptr2chunk(bin);
}


extern _pmalloc_state _pmalloc_main_arena;
extern bool _pmalloc_initialized;


void _pmalloc_initialize(void);
_pmalloc_state* _pmalloc_get_arena(void);
