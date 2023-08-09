#pragma once

#include <pmalloc/type.h>

#ifdef PMALLOC_EXPORTS
	#define PMALLOCAPI __declspec(dllexport)
#else
	#define PMALLOCAPI __declspec(dllimport)
#endif

#define in
#define out


PMALLOCAPI void* pmalloc(in size_t size);
PMALLOCAPI void* pcalloc(in size_t size);
PMALLOCAPI void* prealloc(in out void* ptr, in size_t size);

PMALLOCAPI void* pmalloc_aligned(in size_t size, in size_t alignment);
PMALLOCAPI void* pcalloc_aligned(in size_t size, in size_t alignment);
PMALLOCAPI void* prealloc_aligned(in out void* ptr, in size_t size, in size_t alignment);

PMALLOCAPI void pfree(in void* ptr);
PMALLOCAPI void pfree_aligned(in void* ptr);
