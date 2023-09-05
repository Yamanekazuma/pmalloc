#include "pmalloc.h"
#include <string.h>

#define NOT_POWER_OF_TWO(n) ((n) & ((n) - 1))
#define GET_ALIGNMENT_PTR(original_ptr, alignment) (void*)(((uintptr_t)(original_ptr)&~((uintptr_t)((alignment))-1))+sizeof(void*)+alignment)
// void*のalignmentに従ったptrの一つ手前の領域にoriginal_ptrを格納する
#define ORIGINAL_PTR(ptr) (*((void**)((uintptr_t)(ptr) & ~(uintptr_t)sizeof(void*))-1))

void* pmalloc_aligned(in size_t size, in size_t alignment) {
	if (NOT_POWER_OF_TWO(alignment)) {
		return NULL;
	}
	if (size == 0) {
		return NULL;
	}

	// size + alignment調整 + 元ポインタ保存領域
	void* original_ptr = pmalloc(sizeof(void*) + alignment + size);
	if (original_ptr == NULL) {
		return NULL;
	}
	void* ptr = GET_ALIGNMENT_PTR(original_ptr, alignment);
	// 元ポインタをalignment後から見て-4byte目に保存（freeで使用）
	ORIGINAL_PTR(ptr) = original_ptr;
	return ptr;
}

void pfree_aligned(in void* ptr) {
	if (ptr != NULL) {
		pfree(ORIGINAL_PTR(ptr));
	}
}

void* pcalloc_aligned(in size_t size, in size_t alignment) {
	if (NOT_POWER_OF_TWO(alignment)) {
		return NULL;
	}
	if (size == 0) {
		return NULL;
	}

	void* original_ptr = pcalloc(sizeof(void*) + alignment + size);
	if (original_ptr == NULL) {
		return NULL;
	}
	void* ptr = GET_ALIGNMENT_PTR(original_ptr, alignment);
	ORIGINAL_PTR(ptr) = original_ptr;
	return ptr;
}

void* prealloc_aligned(in out void* ptr, in size_t size, in size_t alignment) {
	if (ptr == NULL) {
		return pmalloc_aligned(size, alignment);
	}

	if (NOT_POWER_OF_TWO(alignment)) {
		return NULL;
	}
	if (size == 0) {
		pfree_aligned(ptr);
		return NULL;
	}

	void* original_ptr = ORIGINAL_PTR(ptr);
	if (ptr != GET_ALIGNMENT_PTR(original_ptr, alignment)) {
		return NULL;
	}
	// alignmentのサイズ次第でptrとoriginal_ptrの間のサイズが変わることがある
	// その際は単純にreallocの再配置に任せると、original_ptrやユーザデータの
	// alignmentが変わってしまうため、修正する必要がある
	ptrdiff_t shift = (char*)ptr - (char*)original_ptr;

	size_t new_size = sizeof(void*) + alignment + size;
	void* new_original_ptr = prealloc(original_ptr, new_size);
	if (new_original_ptr == NULL) {
		return NULL;
	}
	void* new_ptr = GET_ALIGNMENT_PTR(new_original_ptr, alignment);

	// alignmentの修正
	if (shift != (char*)new_ptr - (char*)new_original_ptr) {
		memmove(new_ptr, (char*)new_original_ptr + shift, size);
	}
	ORIGINAL_PTR(new_ptr) = new_original_ptr;
	return new_ptr;
}

#undef ORIGINAL_PTR
#undef GET_ALIGNMENT_PTR
#undef NOT_POWER_OF_TWO
