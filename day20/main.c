#include <aux.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  uint64_t from;
  uint64_t to;
} range;

#define AUX_T range
#define AUX_T_NAME Range
#include <aux_array.h>

static void parse_line(char *line, size_t length, void *userData) {
  range r = {0};
  r.from = (uint64_t)strtol(line, &line, 10);
  r.to = (uint64_t)strtol(line + 1, NULL, 10);
  AuxArrayRangePush(userData, r);
}

static int compare_range(const void *const a, const void *const b) {
  const range ra = *(range *)a;
  const range rb = *(range *)b;
  if (ra.from > rb.from)
    return 1;
  else if (ra.from < rb.from)
    return -1;
  else {
    if (ra.to > rb.to)
      return 1;
    else
      return -1;
  }
}

static uint64_t solve_part1(const AuxArrayRange *const ranges) {
  uint64_t lowest = 0;
  for (size_t i = 0; i < ranges->length; ++i) {
    const range *const r = &ranges->items[i];
    if (r->from > lowest)
      break;
    else
      lowest = r->to + 1;
  }
  return lowest;
}

static uint64_t solve_part2(const AuxArrayRange *const ranges) {
  uint64_t current = 0;
  uint64_t allowed = 0;
  for (size_t i = 0; i < ranges->length; ++i) {
    const range *const r = &ranges->items[i];
    if (r->from > current) {
      allowed += r->from - current;
      current = r->to + 1;
    } else if (r->to > current)
      current = r->to + 1;
  }
  return allowed;
}

int main(void) {
  AuxArrayRange ranges = {0};
  AuxArrayRangeCreate(&ranges, 1080);
  AuxReadFileLineByLine("day20/input.txt", parse_line, &ranges);
  qsort(ranges.items, ranges.length, sizeof(range), compare_range);

  const uint64_t part1 = solve_part1(&ranges);
  const uint64_t part2 = solve_part2(&ranges);

  printf("%lu\n", part1);
  printf("%lu\n", part2);

  AuxArrayRangeDestroy(&ranges);
}
