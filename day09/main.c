#include <aoc/aoc.h>
#include <stdlib.h>
#include <stdio.h>

static inline char *parse_marker(char *input, int32_t *markerLength,
                                 int32_t *repetition) {
  input++; // (
  *markerLength = strtol(input, &input, 10);
  input++; // x
  *repetition = strtol(input, &input, 10);
  input++; // )
  return input;
}

static size_t solve_part1(char *input, const size_t length) {
  char *s = input;
  size_t result = 0;
  while ((s - input) < length) {
    if (*s == '(') {
      int32_t markerLength, repetition;
      s = parse_marker(s, &markerLength, &repetition);
      result += markerLength * repetition;
      s += markerLength;
    } else {
      // this case doesn't happen for the actual input but it's defined in the
      // examples
      s++;
      result++;
    }
  }
  return result;
}

static size_t solve_part2(char *input, const size_t length) {
  char *s = input;
  size_t result = 0;
  while ((s - input) < length) {
    if (*s != '(') {
      s++;
      result++;
    } else {
      int32_t markerLength, repetition;
      s = parse_marker(s, &markerLength, &repetition);
      result += solve_part2(s, markerLength) * repetition;
      s += markerLength;
    }
  }
  return result;
}

int main(void) {
  char *input = NULL;
  size_t length;
  if (!AocReadFileToString("day09/input.txt", &input, &length)) {
    return EXIT_FAILURE;
  }
  AocTrimRight(input, &length);

  const size_t part1 = solve_part1(input, length);
  const size_t part2 = solve_part2(input, length);

  printf("%zu\n", part1);
  printf("%zu\n", part2);

  free(input);
}
