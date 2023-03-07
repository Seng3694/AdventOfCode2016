#ifndef AOC_AUX_COMMON_H
#define AOC_AUX_COMMON_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef NDEBUG

#include <stdio.h>
#include <stdarg.h>
#define AUX_LOG(...) printf(__VA_ARGS__);
#else
#define AUX_LOG(...)
#endif

#endif
