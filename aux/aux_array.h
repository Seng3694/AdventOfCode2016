#include <stdlib.h>

#define COMBINE(a, b) a##b
#define COMBINE2(a, b) COMBINE(a, b)

#ifndef ARRAY_T
#error "ARRAY_T must be defined"
#endif

#ifndef ARRAY_T_NAME
#error "ARRAY_T_NAME must be defined"
#endif

#define ARRAY_NAME COMBINE2(AuxArray, ARRAY_T_NAME)
#define ARRAY_IMPL(word) COMBINE2(ARRAY_NAME, word)
#define ARRAY_LINKAGE static inline
#define ARRAY_SIZE_T size_t

typedef struct {
  ARRAY_SIZE_T capacity;
  ARRAY_SIZE_T length;
  ARRAY_T *items;
} ARRAY_NAME;

#define ARRAY_CREATE ARRAY_IMPL(Create)
#define ARRAY_DESTROY ARRAY_IMPL(Destroy)
#define ARRAY_PUSH ARRAY_IMPL(Push)
#define ARRAY_POP ARRAY_IMPL(Pop)
#define ARRAY_FIRST ARRAY_IMPL(First)
#define ARRAY_LAST ARRAY_IMPL(Last)
#define ARRAY_ENSURE_CAPACITY ARRAY_IMPL(EnsureCapacity)
#define ARRAY_RESIZE ARRAY_IMPL(Resize)
#define ARRAY_CLEAR ARRAY_IMPL(Clear)

ARRAY_LINKAGE void ARRAY_CREATE(ARRAY_NAME *arr, const ARRAY_SIZE_T capacity);
ARRAY_LINKAGE void ARRAY_DESTROY(ARRAY_NAME *arr);
ARRAY_LINKAGE void ARRAY_PUSH(ARRAY_NAME *arr, ARRAY_T item);
ARRAY_LINKAGE void ARRAY_POP(ARRAY_NAME *arr);
ARRAY_LINKAGE ARRAY_T *ARRAY_FIRST(ARRAY_NAME *arr);
ARRAY_LINKAGE ARRAY_T *ARRAY_LAST(ARRAY_NAME *arr);
ARRAY_LINKAGE void ARRAY_ENSURE_CAPACITY(ARRAY_NAME *arr,
                                         const ARRAY_SIZE_T capacity);
ARRAY_LINKAGE void ARRAY_RESIZE(ARRAY_NAME *arr, const ARRAY_SIZE_T capacity);
ARRAY_LINKAGE void ARRAY_CLEAR(ARRAY_NAME *arr);

ARRAY_LINKAGE void ARRAY_CREATE(ARRAY_NAME *arr, const ARRAY_SIZE_T capacity) {
  arr->capacity = capacity;
  arr->length = 0;
  arr->items = (ARRAY_T *)malloc(sizeof(ARRAY_T) * capacity);
}

ARRAY_LINKAGE void ARRAY_DESTROY(ARRAY_NAME *arr) {
  free(arr->items);
  arr->items = NULL;
}

ARRAY_LINKAGE void ARRAY_PUSH(ARRAY_NAME *arr, ARRAY_T item) {
  ARRAY_ENSURE_CAPACITY(arr, arr->length + 1);
  arr->items[arr->length] = item;
  arr->length++;
}

ARRAY_LINKAGE void ARRAY_POP(ARRAY_NAME *arr) {
  if (arr->length == 0)
    return;
  arr->length--;
}

ARRAY_LINKAGE ARRAY_T *ARRAY_FIRST(ARRAY_NAME *arr) {
  return arr->length > 0 ? &arr->items[0] : NULL;
}

ARRAY_LINKAGE ARRAY_T *ARRAY_LAST(ARRAY_NAME *arr) {
  return arr->length > 0 ? &arr->items[arr->length - 1] : NULL;
}

ARRAY_LINKAGE void ARRAY_ENSURE_CAPACITY(ARRAY_NAME *arr,
                                         const ARRAY_SIZE_T capacity) {
  if (arr->capacity < capacity) {
    ARRAY_RESIZE(arr, arr->capacity * 2);
  }
}
ARRAY_LINKAGE void ARRAY_RESIZE(ARRAY_NAME *arr, const ARRAY_SIZE_T capacity) {
  arr->capacity = capacity;
  arr->items = (ARRAY_T *)realloc(arr->items, sizeof(ARRAY_T) * capacity);
}

ARRAY_LINKAGE void ARRAY_CLEAR(ARRAY_NAME *arr) {
  arr->length = 0;
}

#undef COMBINE
#undef COMBINE2
#undef ARRAY_T
#undef ARRAY_T_NAME
#undef ARRAY_NAME
#undef ARRAY_IMPL
#undef ARRAY_LINKAGE
#undef ARRAY_SIZE_T
#undef ARRAY_CREATE
#undef ARRAY_DESTROY
#undef ARRAY_PUSH
#undef ARRAY_POP
#undef ARRAY_FIRST
#undef ARRAY_LAST
#undef ARRAY_ENSURE_CAPACITY
#undef ARRAY_RESIZE
#undef ARRAY_CLEAR
