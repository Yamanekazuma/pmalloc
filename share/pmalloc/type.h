#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef _MSC_VER
#define ATTRIBUTE_TLS __declspec(thread)
#define alignas _Alignas
#else
#include <stdalign.h>
#include <threads.h>
#define ATTRIBUTE_TLS thread_local
#endif
