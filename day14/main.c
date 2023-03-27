#include <aux.h>
#include <aux_md5.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const char hex_lookup[] = "0123456789abcdef";

static inline char get_hash_char(const uint8_t *const hash,
                                 const uint8_t digit) {
  const uint8_t subIndex = 1 - (digit % 2);
  return hex_lookup[(hash[digit / 2] & (0x0f << (4 * subIndex))) >>
                    (4 * subIndex)];
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

static size_t solve_part1(const char *const input) {
  size_t index = 0;
  size_t amount = 0;
  char buffer[256];
  uint8_t hashBuffer[16];

  for (; amount != 64; ++index) {
    size_t length = (size_t)snprintf(buffer, 256, "%s%zu", input, index);
    AuxMD5(buffer, length, hashBuffer);
    char c;

    if (contains_multiple(hashBuffer, 3, &c)) {
      size_t next = 1;
      for (; next <= 1000; ++next) {
        length = (size_t)snprintf(buffer, 256, "%s%zu", input, index + next);
        AuxMD5(buffer, length, hashBuffer);
        char c2;

        if (contains_multiple(hashBuffer, 5, &c2) && c2 == c) {
          amount++;
          break;
        }
      }
    }
  }

  return index - 1;
}

int main(void) {
  const char *const input = "jlmsuwbz";
  const size_t part1 = solve_part1(input);
  printf("%zu\n", part1);
}
