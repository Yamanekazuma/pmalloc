#pragma once

#include <stdint.h>

typedef enum _MEMORY_PROTECT {
	MP_R,
	MP_RW,
	MP_RX,
	MP_RWX,
} MEMORY_PROTECT;

typedef MEMORY_PROTECT memory_protect_t;
