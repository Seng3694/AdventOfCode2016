#include <aux.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  uint8_t position;
  uint8_t count;
} disc;

#define AUX_T disc
#define AUX_T_NAME Disc
#include <aux_array.h>

static void parse_line(char *line, size_t length, void *userData) {
  disc d = {0};
  line += 12; // skip "Disc #X has "
  d.count = (uint8_t)strtol(line, &line, 10);
  line += 41; // skip " positions; at time=0, it is at position "
  d.position = (uint8_t)strtol(line, NULL, 10);
  AuxArrayDiscPush(userData, d);
}

static bool simulate(const AuxArrayDisc *const discs, uint32_t timer) {
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

static uint32_t solve(const AuxArrayDisc *const discs) {
  for (uint32_t i = 0;; ++i) {
    if (simulate(discs, i))
      return i;
  }
  return 0; // should never reach
}

int main(void) {
  AuxArrayDisc discs = {0};
  AuxArrayDiscCreate(&discs, 6);
  AuxReadFileLineByLine("day15/input.txt", parse_line, &discs);

  const uint32_t part1 = solve(&discs);

  printf("%u\n", part1);

  AuxArrayDiscDestroy(&discs);
}
