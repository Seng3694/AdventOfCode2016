#include <aoc/aoc.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  uint8_t position;
  uint8_t count;
} disc;

#define AOC_T disc
#define AOC_T_NAME Disc
#include <aoc/array.h>

static void parse_line(char *line, size_t length, void *userData) {
  disc d = {0};
  line += 12; // skip "Disc #X has "
  d.count = (uint8_t)strtol(line, &line, 10);
  line += 41; // skip " positions; at time=0, it is at position "
  d.position = (uint8_t)strtol(line, NULL, 10);
  AocArrayDiscPush(userData, d);
}

static bool simulate(const AocArrayDisc *const discs, uint32_t timer) {
  const uint8_t goal = (uint8_t)discs->length;
  uint8_t position = 0;
  do {
    timer++;
    if (((discs->items[position].position + timer) %
         discs->items[position].count) != 0) {
      return false;
    }
    position++;
  } while (position < goal);
  return true;
}

static uint32_t solve(const AocArrayDisc *const discs, const uint32_t start) {
  for (uint32_t i = start;; ++i) {
    if (simulate(discs, i))
      return i;
  }
  return 0; // should never reach
}

int main(void) {
  AocArrayDisc discs = {0};
  AocArrayDiscCreate(&discs, 7);
  AocReadFileLineByLine("day15/input.txt", parse_line, &discs);

  const uint32_t part1 = solve(&discs, 0);

  AocArrayDiscPush(&discs, (disc){.count = 11, .position = 0});
  const uint32_t part2 = solve(&discs, part1);

  printf("%u\n", part1);
  printf("%u\n", part2);

  AocArrayDiscDestroy(&discs);
}
