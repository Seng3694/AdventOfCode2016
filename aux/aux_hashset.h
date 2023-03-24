#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

// these macros have to be defined
// #define AUX_T
// #define AUX_T_NAME
// #define AUX_T_EMPTY
// #define AUX_T_HFUNC(x)
// #define AUX_T_EQUALS(a, b)

#ifndef AUX_T
#error "AUX_T must be defined"
#endif

#ifndef AUX_T_NAME
#error "AUX_T_NAME must be defined"
#endif

#ifndef AUX_T_EMPTY
#error "AUX_T_EMPTY must be defined"
#endif

#ifndef AUX_T_HFUNC
#error "AUX_T_HFUNC must be defined"
#endif

#ifndef AUX_T_EQUALS
#error "AUX_T_EQUALS must be defined"
#endif

#define COMBINE(a, b) a##b
#define COMBINE2(a, b) COMBINE(a, b)

#define HS_NAME COMBINE2(AuxHashset, AUX_T_NAME)
#define HS_IMPL(word) COMBINE2(HS_NAME, word)
#define HS_LINKAGE static inline
#define HS_SIZE_T size_t
#define HS_MAX_LOAD 0.75

typedef struct {
  HS_SIZE_T capacity;
  HS_SIZE_T count;
  AUX_T *entries;
} HS_NAME;

#define HS_CREATE HS_IMPL(Create)
#define HS_DESTROY HS_IMPL(Destroy)
#define HS_ADJUST_CAP HS_IMPL(AdjustCapacity)
#define HS_INSERT_INTERNAL HS_IMPL(InsertInternal)
#define HS_INSERT HS_IMPL(Insert)
#define HS_INSERT_PH HS_IMPL(InsertPreHashed)
#define HS_REMOVE HS_IMPL(Remove)
#define HS_REMOVE_PH HS_IMPL(RemovePreHashed)
#define HS_CLEAR HS_IMPL(Clear)
#define HS_CONTAINS HS_IMPL(Contains)

HS_LINKAGE void HS_CREATE(HS_NAME *const hs, const HS_SIZE_T capacity);
HS_LINKAGE void HS_DESTROY(HS_NAME *hs);
HS_LINKAGE void HS_ADJUST_CAP(HS_NAME *hs, const HS_SIZE_T capacity);

HS_LINKAGE void HS_INSERT_INTERNAL(AUX_T *entries, const HS_SIZE_T capacity,
                                   const AUX_T key, const uint32_t *hash);
HS_LINKAGE void HS_INSERT(HS_NAME *const hs, const AUX_T key);
HS_LINKAGE void HS_INSERT_PH(HS_NAME *const hs, const AUX_T key,
                             const uint32_t hash);

HS_LINKAGE void HS_REMOVE(HS_NAME *const hs, const AUX_T key);
HS_LINKAGE void HS_REMOVE_PH(HS_NAME *const hs, const AUX_T key,
                             const uint32_t hash);

HS_LINKAGE void HS_CLEAR(HS_NAME *const hs);

HS_LINKAGE bool HS_CONTAINS(const HS_NAME *const hs, const AUX_T key,
                            uint32_t *hash);

HS_LINKAGE void HS_CREATE(HS_NAME *const hs, const HS_SIZE_T capacity) {
  hs->capacity = capacity;
  hs->count = 0;
  hs->entries = (AUX_T *)malloc(sizeof(AUX_T) * hs->capacity);
  for (HS_SIZE_T i = 0; i < capacity; ++i) {
    hs->entries[i] = AUX_T_EMPTY;
  }
}

HS_LINKAGE void HS_DESTROY(HS_NAME *hs) {
  free(hs->entries);
}

HS_LINKAGE void HS_ADJUST_CAP(HS_NAME *hs, const HS_SIZE_T capacity) {
  AUX_T *entries = (AUX_T *)malloc(sizeof(AUX_T) * capacity);
  for (HS_SIZE_T i = 0; i < capacity; ++i) {
    entries[i] = AUX_T_EMPTY;
  }

  hs->count = 0;
  for (HS_SIZE_T i = 0; i < hs->capacity; ++i) {
    AUX_T *entry = &hs->entries[i];
    if (AUX_T_EQUALS(entry, &AUX_T_EMPTY))
      continue;
    HS_INSERT_INTERNAL(entries, capacity, *entry, NULL);
    hs->count++;
  }

  free(hs->entries);
  hs->entries = entries;
  hs->capacity = capacity;
}

HS_LINKAGE void HS_INSERT_INTERNAL(AUX_T *entries, const HS_SIZE_T capacity,
                                   const AUX_T key, const uint32_t *hash) {
  const uint32_t keyHash = hash ? *hash : AUX_T_HFUNC(&key);
  uint32_t index = keyHash & (capacity - 1);
  for (;;) {
    AUX_T *entry = &entries[index];
    if (AUX_T_EQUALS(entry, &AUX_T_EMPTY)) {
      *entry = key;
      return;
    }
    index = (index + 1) & (capacity - 1);
  }
}

HS_LINKAGE void HS_INSERT(HS_NAME *const hs, const AUX_T key) {
  if (hs->count + 1 > hs->capacity * HS_MAX_LOAD) {
    const HS_SIZE_T capacity = hs->capacity * 2;
    HS_ADJUST_CAP(hs, capacity);
  }
  HS_INSERT_INTERNAL(hs->entries, hs->capacity, key, NULL);
  hs->count++;
}

HS_LINKAGE void HS_INSERT_PH(HS_NAME *const hs, const AUX_T key,
                             const uint32_t hash) {
  if (hs->count + 1 > hs->capacity * HS_MAX_LOAD) {
    const HS_SIZE_T capacity = hs->capacity * 2;
    HS_ADJUST_CAP(hs, capacity);
  }
  HS_INSERT_INTERNAL(hs->entries, hs->capacity, key, &hash);
  hs->count++;
}

HS_LINKAGE void HS_REMOVE(HS_NAME *const hs, const AUX_T key) {
  const uint32_t keyHash = AUX_T_HFUNC(&key);
  HS_REMOVE_PH(hs, key, keyHash);
}

HS_LINKAGE void HS_REMOVE_PH(HS_NAME *const hs, const AUX_T key,
                             const uint32_t hash) {
  uint32_t index = hash & (hs->capacity - 1);
  for (;;) {
    AUX_T *entry = &hs->entries[index];
    if (AUX_T_EQUALS(entry, &AUX_T_EMPTY)) {
      return;
    } else if (AUX_T_EQUALS(entry, &key)) {
      *entry = AUX_T_EMPTY;
      hs->count--;
      return;
    }
    index = (index + 1) & (hs->capacity - 1);
  }
}

HS_LINKAGE void HS_CLEAR(HS_NAME *const hs) {
  for (HS_SIZE_T i = 0; i < hs->capacity; ++i) {
    hs->entries[i] = AUX_T_EMPTY;
  }
  hs->count = 0;
}

HS_LINKAGE bool HS_CONTAINS(const HS_NAME *const hs, const AUX_T key,
                            uint32_t *hash) {
  const uint32_t keyHash = AUX_T_HFUNC(&key);
  if (hash)
    *hash = keyHash;
  uint32_t index = keyHash & (hs->capacity - 1);
  for (;;) {
    AUX_T *entry = &hs->entries[index];
    if (AUX_T_EQUALS(entry, &AUX_T_EMPTY)) {
      return false;
    } else if (AUX_T_EQUALS(entry, &key)) {
      return true;
    }
    index = (index + 1) & (hs->capacity - 1);
  }
}

#undef COMBINE
#undef COMBINE2
#undef AUX_T
#undef AUX_T_NAME
#undef AUX_T_EMPTY
#undef AUX_T_HFUNC
#undef AUX_T_EQUALS
#undef HS_NAME
#undef HS_IMPL
#undef HS_LINKAGE
#undef HS_SIZE_T
#undef HS_MAX_LOAD
#undef HS_CREATE
#undef HS_DESTROY
#undef HS_ADJUST_CAP
#undef HS_INSERT_INTERNAL
#undef HS_INSERT
#undef HS_INSERT_PH
#undef HS_REMOVE
#undef HS_REMOVE_PH
#undef HS_CLEAR
#undef HS_CONTAINS
