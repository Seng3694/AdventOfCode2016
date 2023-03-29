#include <aux.h>
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

static tile_type uncover_tile_type(const char *const prevRow,
                                   const size_t width, const size_t x) {
  const char pattern[3] = {
      x == 0 ? tileTypes[TILE_SAFE] : prevRow[x - 1],
      prevRow[x],
      x == width - 1 ? tileTypes[TILE_SAFE] : prevRow[x + 1],
  };
  for (size_t i = 0; i < 4; ++i) {
    if (memcmp(pattern, unsafePatterns[i], sizeof(char) * 3) == 0)
      return TILE_TRAP;
  }
  return TILE_SAFE;
}

static uint32_t solve_part1(const char *const input, const size_t width) {
  const size_t height = 40;
  char *const maze = malloc(sizeof(char) * height * width);
  memcpy(maze, input, sizeof(char) * width);
  uint32_t safeTiles = 0;
  for (size_t i = 0; i < width; ++i)
    if (maze[i] == tileTypes[TILE_SAFE])
      safeTiles++;

  for (size_t y = 1; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      const tile_type tile =
          uncover_tile_type(&maze[(y - 1) * width], width, x);
      safeTiles += tile;
      maze[y * width + x] = tileTypes[tile];
    }
  }

  free(maze);
  return safeTiles;
}

int main(void) {
  char *input;
  size_t length;
  if (!AuxReadFileToString("day18/input.txt", &input, &length))
    return EXIT_FAILURE;
  AuxRemoveTrailingWhitespace(input, &length);

  const uint32_t part1 = solve_part1(input, length);

  printf("%u\n", part1);

  free(input);
}
