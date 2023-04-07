#include <aoc/aoc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  TILE_TRAP,
  TILE_SAFE,
} tile_type;

static const char tileTypes[2] = {'^', '.'};

static const char unsafePatterns[4][4] = {
    "^^.",
    ".^^",
    "..^",
    "^..",
};

static tile_type uncover_tile_type(const char *const pattern) {
  for (size_t i = 0; i < 4; ++i) {
    if (memcmp(pattern, unsafePatterns[i], sizeof(char) * 3) == 0)
      return TILE_TRAP;
  }
  return TILE_SAFE;
}

static uint32_t uncover_row(const char *const prevRow, char *const row,
                            const size_t width) {
  const char startPattern[3] = {
      tileTypes[TILE_SAFE],
      prevRow[0],
      prevRow[1],
  };
  const char endPattern[3] = {
      prevRow[width - 2],
      prevRow[width - 1],
      tileTypes[TILE_SAFE],
  };

  tile_type type = uncover_tile_type(startPattern);
  row[0] = tileTypes[type];
  uint32_t safeTiles = type;

  type = uncover_tile_type(endPattern);
  row[width - 1] = tileTypes[type];
  safeTiles += type;

  const size_t dest = width - 1;
  for (size_t i = 1; i < dest; ++i) {
    type = uncover_tile_type(prevRow - 1 + i);
    row[i] = tileTypes[type];
    safeTiles += type;
  }

  return safeTiles;
}

static inline void swap(char **a, char **b) {
  char *tmp = *a;
  *a = *b;
  *b = tmp;
}

static uint32_t solve(const char *const input, const size_t width,
                      const size_t height) {
  char *const rows = malloc(sizeof(char) * width * 2);
  memcpy(rows, input, sizeof(char) * width);
  uint32_t safeTiles = 0;
  for (size_t i = 0; i < width; ++i)
    if (rows[i] == tileTypes[TILE_SAFE])
      safeTiles++;

  char *prevRow = rows;
  char *row = rows + width;

  for (size_t y = 1; y < height; ++y) {
    safeTiles += uncover_row(prevRow, row, width);
    swap(&prevRow, &row);
  }

  free(rows);
  return safeTiles;
}

int main(void) {
  char *input;
  size_t length;
  if (!AocReadFileToString("day18/input.txt", &input, &length))
    return EXIT_FAILURE;
  AocTrimRight(input, &length);

  const uint32_t part1 = solve(input, length, 40);
  const uint32_t part2 = solve(input, length, 400000);

  printf("%u\n", part1);
  printf("%u\n", part2);

  free(input);
}
