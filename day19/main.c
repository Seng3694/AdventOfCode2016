#include <stdio.h>
#include <stdint.h>
#include <math.h>

static uint32_t highest_bit(const uint32_t n) {
  return 1 << (uint32_t)log2((double)n);
}

static uint32_t solve_part1(const uint32_t n) {
  // Josephus Problem: https://www.youtube.com/watch?v=uCsD3ZGzMgE
  // move leading bit to the end
  return (n ^ highest_bit(n)) << 1 | 1;
}

int main(void) {
  const uint32_t input = 3004953;
  const uint32_t part1 = solve_part1(input);
  printf("%u\n", part1);
}
