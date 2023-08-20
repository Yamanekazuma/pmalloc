#pragma once

#include <pmalloc/type.h>

#define in
#define out


void* pmalloc(in size_t size);
void* pcalloc(in size_t size);
void* prealloc(in out void* ptr, in size_t size);

void* pmalloc_aligned(in size_t size, in size_t alignment);
void* pcalloc_aligned(in size_t size, in size_t alignment);
void* prealloc_aligned(in out void* ptr, in size_t size, in size_t alignment);

void pfree(in void* ptr);
void pfree_aligned(in void* ptr);
