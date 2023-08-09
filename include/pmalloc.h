#pragma once

#include "Windows.h"

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

PMALLOCAPI void* pmalloc_aligned(in size_t size, in size_t alignemnt);
PMALLOCAPI void* pcalloc_aligned(in size_t size, in size_t alignemnt);
PMALLOCAPI void* prealloc_aligned(in out void* ptr, in size_t alignemnt);

PMALLOCAPI void pfree(in void* ptr);
