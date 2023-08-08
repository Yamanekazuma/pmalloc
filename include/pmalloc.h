#pragma once

#include "pmalloc/type.h"


#ifdef PMALLOC_EXPORTS
	#define PMALLOCAPI __declspec(dllexport)
#else
	#define PMALLOCAPI __declspec(dllimport)
#endif

#define in
#define out


PMALLOCAPI void* pmalloc(in size_t size, in memory_protect_t protect);
PMALLOCAPI void* pcalloc(in size_t size, in memory_protect_t protect);
PMALLOCAPI void* prealloc(in void* ptr, in memory_protect_t protect, in size_t size);
PMALLOCAPI void pfree(in void* ptr);
