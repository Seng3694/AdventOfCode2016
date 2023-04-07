#include <aoc/aoc.h>
#include <aoc/md5.h>
#include <string.h>
#include <stdio.h>

static const char hex_lookup[] = "0123456789abcdef";

static inline char get_hash_char(const uint8_t *const hash,
                                 const uint8_t digit) {
  const uint8_t subIndex = 4 * (1 - (digit & 1));
  return hex_lookup[(hash[digit >> 1] & (0x0f << subIndex)) >> subIndex];
}

static inline void hash_to_string(const uint8_t *const hash,
                                  char *const buffer) {
  for (uint8_t i = 0; i < 32; ++i)
    buffer[i] = get_hash_char(hash, i);
}

static bool contains_multiple(const uint8_t *const hash, const size_t amount,
                              char *const c) {
  for (size_t i = 0; i < 32; i++) {
    if (32 - i < amount) {
      for (size_t j = 0; j < amount - 1; ++j) {
        if (get_hash_char(hash, 32 - j - 1) != get_hash_char(hash, 32 - j - 2))
          goto next;
      }
    } else {
      for (size_t j = 0; j < amount - 1; ++j) {
        if (get_hash_char(hash, i + j) != get_hash_char(hash, i + j + 1))
          goto next;
      }
    }
    *c = get_hash_char(hash, i);
    return true;
  next:;
  }
  return false;
}

typedef struct {
  size_t index;
  uint16_t ttl;
  uint8_t hash[16];
  char c;
} search_data;

#define AOC_T search_data
#define AOC_T_NAME SearchData
#include <aoc/array.h>

#define AOC_T size_t
#define AOC_T_NAME Indexes
#include <aoc/array.h>

static int size_comparer(const void *const a, const void *const b) {
  return (*(size_t *)a) - (*(size_t *)b);
}

static void process_search_data(AocArraySearchData *const hashes,
                                AocArrayIndexes *const indexes,
                                const uint8_t *const hashBuffer) {
  for (int i = hashes->length - 1; i >= 0; --i) {
    if (i < 0)
      break;

    search_data *const data = &hashes->items[i];
    bool found = false;
    char c;
    if (contains_multiple(hashBuffer, 5, &c) && c == data->c) {
      AocArrayIndexesPush(indexes, data->index);
      found = true;
    }

    data->ttl--;
    if (data->ttl == 0 || found) {
      memcpy(hashes->items + i, hashes->items + hashes->length - 1,
             sizeof(search_data));
      hashes->length--;
    }
  }
}

static size_t solve(const char *const input, bool part2) {
  char buffer[256];
  uint8_t hashBuffer[16];
  char hashStringBuffer[33] = {0};

  AocArraySearchData hashes = {0};
  AocArraySearchDataCreate(&hashes, 1000);

  AocArrayIndexes indexes = {0};
  AocArrayIndexesCreate(&indexes, 128);

  for (size_t index = 0; indexes.length < 64; ++index) {
    size_t length = (size_t)snprintf(buffer, 256, "%s%zu", input, index);
    AocMD5(buffer, length, hashBuffer);

    if (part2) {
      hash_to_string(hashBuffer, hashStringBuffer);
      for (size_t i = 0; i < 2016; ++i) {
        AocMD5(hashStringBuffer, 32, hashBuffer);
        hash_to_string(hashBuffer, hashStringBuffer);
      }
    }

    process_search_data(&hashes, &indexes, hashBuffer);

    char c;
    if (contains_multiple(hashBuffer, 3, &c)) {
      search_data data = {.ttl = 1000, .c = c, .index = index};
      memcpy(data.hash, hashBuffer, 16 * sizeof(uint8_t));
      AocArraySearchDataPush(&hashes, data);
    }
  }

  // process remaining hashes because they are out of order
  while (hashes.length > 0) {
    process_search_data(&hashes, &indexes, hashBuffer);
  }

  qsort(indexes.items, indexes.length, sizeof(size_t), size_comparer);
  const size_t solution = indexes.items[63];

  AocArrayIndexesDestroy(&indexes);
  AocArraySearchDataDestroy(&hashes);
  return solution;
}

int main(void) {
  const char *const input = "jlmsuwbz";
  const size_t part1 = solve(input, false);
  const size_t part2 = solve(input, true);
  printf("%zu\n", part1);
  printf("%zu\n", part2);
}
