#include <aux_md5.h>
#include <string.h>
#include <stdio.h>

static const char hex_lookup[] = "0123456789abcdef";

static inline bool starts_with_5_zeroes(const uint8_t *const hash) {
  return hash[0] == 0 && hash[1] == 0 && ((hash[2] & 0xf0) == 0);
}

static inline char get_sixth_hash_digit(const uint8_t *const hash) {
  return hex_lookup[hash[2] & 0xf];
}

static void solve_part1(const char input[], const size_t inputLength,
                        char *const output) {
  // buffer to store the string to hash
  char buffer[256] = {0};
  memcpy(buffer, input, inputLength);
  // points to where the numbers will be written to
  char *start = buffer + inputLength;
  size_t solutionLength = 0;
  // buffer for the md5 hashes
  uint8_t hashResult[16] = {0};
  for (size_t i = 0; solutionLength < 8; ++i) {
    sprintf(start, "%zu", i);
    AuxMD5(buffer, strlen(buffer), hashResult);
    if (starts_with_5_zeroes(hashResult)) {
      output[solutionLength++] = get_sixth_hash_digit(hashResult);
    }
  }
}

int main(void) {
  const char input[] = "wtnhxymk";
  const size_t inputLength = sizeof(input) - 1;
  char part1[9] = {0};
  solve_part1(input, inputLength, part1);

  printf("%s\n", part1);
}