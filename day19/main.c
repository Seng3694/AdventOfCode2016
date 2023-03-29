#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

static inline uint32_t find_next(const uint32_t *const gifts, uint32_t current,
                                 const uint32_t count) {
  do {
    current = (current + 1) % count;
  } while (gifts[current] == 0);
  return current;
}

static uint32_t solve_part1(const uint32_t elves) {
  uint32_t *gifts = malloc(sizeof(uint32_t) * elves);
  for (uint32_t i = 0; i < elves; ++i) {
    gifts[i] = 1;
  }
  const uint32_t runs = (uint32_t)log2((double)elves);

  uint32_t last = 0;
  for (uint32_t i = 0; i < runs; ++i) {
    for (uint32_t e = 0; e < elves; ++e) {
      if (gifts[e] == 0) {
        continue;
      }
      const size_t next = find_next(gifts, e, elves);
      gifts[e] += gifts[next];
      gifts[next] = 0;
      last = e;
    }
  }

  return last + 1;
}

int main(void) {
  const uint32_t input = 3004953;
  const uint32_t part1 = solve_part1(input);
  printf("%u\n", part1);
}
