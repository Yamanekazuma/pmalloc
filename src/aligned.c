#include "pmalloc.h"

#define NOT_POWER_OF_TWO(n) ((n) & ((n) - 1))
#define GET_ALIGNMENT_PTR(original_ptr, alignment) (void*)(((uintptr_t)(original_ptr)&~((uintptr_t)((alignment))-1))+sizeof(void*)+alignment)
// void*��alignment�ɏ]����ptr�̈��O�̗̈��original_ptr���i�[����
#define ORIGINAL_PTR(ptr) (*((void**)((uintptr_t)(ptr) & ~(uintptr_t)sizeof(void*))-1))

void* pmalloc_aligned(in size_t size, in size_t alignment) {
	if (NOT_POWER_OF_TWO(alignment)) {
		return NULL;
	}
	if (size == 0) {
		return NULL;
	}

	// size + alignment���� + ���|�C���^�ۑ��̈�
	void* original_ptr = pmalloc(sizeof(void*) + alignment + size);
	if (original_ptr == NULL) {
		return NULL;
	}
	void* ptr = GET_ALIGNMENT_PTR(original_ptr, alignment);
	// ���|�C���^��alignment�ォ�猩��-4byte�ڂɕۑ��ifree�Ŏg�p�j
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
	// alignment�̃T�C�Y�����ptr��original_ptr�̊Ԃ̃T�C�Y���ς�邱�Ƃ�����
	// ���̍ۂ͒P����realloc�̍Ĕz�u�ɔC����ƁAoriginal_ptr�⃆�[�U�f�[�^��
	// alignment���ς���Ă��܂����߁A�C������K�v������
	ptrdiff_t shift = (char*)ptr - (char*)original_ptr;

	void* new_original_ptr = prealloc(original_ptr, sizeof(void*) + alignment + size);
	if (new_original_ptr == NULL) {
		return NULL;
	}
	void* new_ptr = GET_ALIGNMENT_PTR(new_original_ptr, alignment);

	// alignment�̏C��
	if (shift != (char*)new_ptr - (char*)new_original_ptr) {
		memmove(new_ptr, (char*)new_original_ptr + shift, size);
	}
	ORIGINAL_PTR(new_ptr) = new_original_ptr;
	return new_ptr;
}

#undef ORIGINAL_PTR(p)
#undef GET_ALIGNMENT_PTR(p)
#undef NOT_POWER_OF_TWO(n)
