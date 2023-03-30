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

static uint32_t solve_part2(const uint32_t n) {
  const uint32_t power = pow(3, floor(log(n) / log(3)));
  const uint32_t rest = n - power;
  if (n == power)
    return n;
  else if (rest <= power)
    return n - power;
  else
    return 2 * n - 3 * power;
}

int main(void) {
  const uint32_t input = 3004953;
  const uint32_t part1 = solve_part1(input);
  const uint32_t part2 = solve_part2(input);
  printf("%u\n", part1);
  printf("%u\n", part2);
}
