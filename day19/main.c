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

static void print_gifts(const uint32_t *const gifts, const uint32_t count) {
  for (uint32_t i = 0; i < count; ++i) {
    putc(gifts[i] == 0 ? '.' : '#', stdout);
  }
  printf("\n");
}

static uint32_t solve_part1(const uint32_t elves) {
  uint32_t *gifts = malloc(sizeof(uint32_t) * elves);
  for (uint32_t i = 0; i < elves; ++i) {
    gifts[i] = 1;
  }

  const uint32_t rounds = (uint32_t)log2((double)elves);

  uint32_t last = 0;
  uint32_t current = 0;
  for (uint32_t r = 0; r < rounds; ++r) {
    uint32_t step = pow(2, r);

    for (uint32_t e = current; e < elves; e += step) {
      if (gifts[e] == 0) {
        continue;
      }
      current = find_next(gifts, e + step - 1, elves);
      gifts[e] += gifts[current];
      gifts[current] = 0;
      last = e;
    }
    current = find_next(gifts, current, elves);
  }

  free(gifts);
  return last + 1;
}

int main(void) {

  const uint32_t input = 3004953;
  const uint32_t part1 = solve_part1(input);
  printf("%u\n", part1);
}
