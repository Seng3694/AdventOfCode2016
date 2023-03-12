#include <aux_md5.h>
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
      output[solutionLength++] = hex_lookup[get_sixth_hash_digit(hashResult)];
    }
  }
}

static void solve_part2(const char input[], const size_t inputLength,
                        char *const output) {
  // init with sentinel value
  memset(output, '?', 8);
  char buffer[256] = {0};
  memcpy(buffer, input, inputLength);
  char *start = buffer + inputLength;
  size_t solutionLength = 0;
  uint8_t hashResult[16] = {0};
  for (size_t i = 0; solutionLength < 8; ++i) {
    sprintf(start, "%zu", i);
    AuxMD5(buffer, strlen(buffer), hashResult);
    if (starts_with_5_zeroes(hashResult)) {
      const uint8_t index = get_sixth_hash_digit(hashResult);
      if (index < 8 && output[index] == '?') {
        output[index] = hex_lookup[get_seventh_hash_digit(hashResult)];
        solutionLength++;
      }
    }
  }
}

int main(void) {
  const char input[] = "wtnhxymk";
  const size_t inputLength = sizeof(input) - 1;
  char part1[9] = {0};
  char part2[9] = {0};

  solve_part1(input, inputLength, part1);
  solve_part2(input, inputLength, part2);

  printf("%s\n", part1);
  printf("%s\n", part2);
}
