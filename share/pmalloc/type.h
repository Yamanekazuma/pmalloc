#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef _MSC_VER
  #define ATTRIBUTE_TLS __declspec(thread)
  #define alignas _Alignas
#else
  #include <stdalign.h>
  #ifdef thread_local
    #define ATTRIBUTE_TLS thread_local
  #elif __STDC_VERSION__ >= 201112L && !(defined(__STDC_NO_THREADS__) || (defined(__GNU_LIBRARY__) && __GLIBC__ == 2 && __GLIBC_MINOR__ < 16))
    #define ATTRIBUTE_TLS _Thread_local
  #else
    #define ATTRIBUTE_TLS __thread
  #endif
#endif
