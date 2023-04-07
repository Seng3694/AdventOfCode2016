#include <aoc/md5.h>
#include <string.h>
#include <stdio.h>

static const char hex_lookup[] = "0123456789abcdef";

static inline bool starts_with_5_zeroes(const uint8_t *const hash) {
  return hash[0] == 0 && hash[1] == 0 && ((hash[2] & 0xf0) == 0);
}

static inline uint8_t get_sixth_hash_digit(const uint8_t *const hash) {
  return hash[2] & 0xf;
}

static inline uint8_t get_seventh_hash_digit(const uint8_t *const hash) {
  return (hash[3] & 0xf0) >> 4;
}

static void solve_both_parts(const char input[], const size_t inputLength,
                             char *const part1, char *const part2) {
  // init with sentinel value
  memset(part2, '?', 8);
  char buffer[256] = {0};
  memcpy(buffer, input, inputLength);
  char *start = buffer + inputLength;
  size_t solutionLength1 = 0;
  size_t solutionLength2 = 0;
  uint8_t hashResult[16] = {0};
  for (size_t i = 0; solutionLength2 < 8; ++i) {
    sprintf(start, "%zu", i);
    AocMD5(buffer, strlen(buffer), hashResult);
    if (starts_with_5_zeroes(hashResult)) {
      const uint8_t index = get_sixth_hash_digit(hashResult);
      if (solutionLength1 < 8)
        part1[solutionLength1++] = hex_lookup[index];
      if (index < 8 && part2[index] == '?') {
        part2[index] = hex_lookup[get_seventh_hash_digit(hashResult)];
        solutionLength2++;
      }
    }
  }
}

int main(void) {
  const char input[] = "wtnhxymk";
  const size_t inputLength = sizeof(input) - 1;
  char part1[9] = {0};
  char part2[9] = {0};

  solve_both_parts(input, inputLength, part1, part2);

  printf("%s\n", part1);
  printf("%s\n", part2);
}
