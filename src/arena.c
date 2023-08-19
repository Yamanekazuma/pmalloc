#include "arena.h"

static void _pmalloc_init_state(_pmalloc_state* arena);
static _pmalloc_state* _pmalloc_create_next_arena(_pmalloc_state* last_arena);

#define INITIAL_HEAP_SIZE 0x1000
#define GET_ALIGNED_PTR(ptr, alignment) (((uintptr_t)(ptr) + alignment - 1) & ~(alignment - 1))

alignas(_PM_ARENA_ALIGNMENT) _pmalloc_state _pmalloc_main_arena = {0};
bool _pmalloc_initialized = false;

static ATTRIBUTE_TLS _pmalloc_state* _pmalloc_thread_arena = NULL;


void _pmalloc_initialize(void) {
	if (_pmalloc_initialized) {
		return;
	}
	_pmalloc_initialized = true;
	_pmalloc_init_state(&_pmalloc_main_arena);
	_pmalloc_main_arena.heap = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE | HEAP_NO_SERIALIZE, INITIAL_HEAP_SIZE, 0);
}

_pmalloc_state* _pmalloc_get_arena(void) {
	if (_pmalloc_thread_arena != NULL) {
		return _pmalloc_thread_arena;
	}

	_pmalloc_state* arena = &_pmalloc_main_arena;
	if (TryEnterCriticalSection(&(arena->mutex)) != 0) {
		_pmalloc_thread_arena = arena;
		return arena;
	}
	while (arena->next != NULL) {
		arena = arena->next;
		if (TryEnterCriticalSection(&(arena->mutex)) != 0) {
			_pmalloc_thread_arena = arena;
			return arena;
		}
	}

	arena = _pmalloc_create_next_arena(arena);
	_pmalloc_thread_arena = arena;
	return arena;
}


static void _pmalloc_init_state(_pmalloc_state* arena) {
	InitializeCriticalSection(&(arena->mutex));
}

static _pmalloc_state* _pmalloc_create_next_arena(_pmalloc_state* last_arena) {
	assert(last_arena != NULL);
	assert(last_arena->next == NULL);

	HANDLE heap = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE | HEAP_NO_SERIALIZE, INITIAL_HEAP_SIZE, 0);
	if (heap == NULL) {
		return NULL;
	}
	_pmalloc_state* arena = (_pmalloc_state*)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(_pmalloc_state) + _PM_ARENA_ALIGNMENT - 1);
	if (arena == NULL) {
		return NULL;
	}
	else {
		arena = (_pmalloc_state*)GET_ALIGNED_PTR(arena, _PM_ARENA_ALIGNMENT);
		_pmalloc_init_state(arena);
		arena->heap = heap;
		last_arena->next = arena;
		return arena;
	}
}

#undef GET_ALIGNED_PTR
