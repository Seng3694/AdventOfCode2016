#include <aux.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum {
  NODE_TYPE_REGULAR,
  NODE_TYPE_EMPTY,
  NODE_TYPE_FULL,
  NODE_TYPE_DATA,
  NODE_TYPE_COUNT,
} node_type;

typedef struct {
  uint16_t size;
  uint16_t used;
  uint8_t x;
  uint8_t y;
} node;

#define AUX_T node
#define AUX_T_NAME Node
#include <aux_array.h>

typedef struct {
  uint8_t width;
  uint8_t height;
  AuxArrayNode nodes;
} parsing_context;

typedef struct {
  uint8_t x;
  uint8_t y;
} position;

static inline uint32_t calc_position_hash(const position *const pos) {
  return 2166136261u ^ (pos->x * 16777619) ^ (pos->y * 57620617);
}

static inline bool position_equals(const position *const a,
                                   const position *const b) {
  return a->x == b->x && a->y == b->y;
}

#define AUX_T position
#define AUX_T_NAME Pos
#define AUX_T_EMPTY ((position){0xff, 0xff})
#define AUX_T_HFUNC(x) calc_position_hash(x)
#define AUX_T_EQUALS(a, b) position_equals(a, b)
#include <aux_hashset.h>

#define AUX_T position
#define AUX_T_NAME Pos
#include <aux_array.h>

typedef struct {
  uint8_t width;
  uint8_t height;
  position initialEmptyPos;
  position initialDataPos;
  uint8_t *nodes;
} grid;

void grid_init(const parsing_context *const ctx, grid *g) {
  g->nodes = malloc(sizeof(uint8_t) * ctx->width * ctx->height);
  g->width = ctx->width;
  g->height = ctx->height;
  for (size_t i = 0; i < ctx->nodes.length; ++i) {
    const node *const n = &ctx->nodes.items[i];
    const size_t index = n->y * g->width + n->x;
    if (n->x == g->width - 1 && n->y == 0) {
      g->nodes[index] = NODE_TYPE_DATA;
      g->initialDataPos = (position){n->x, n->y};
    } else if (n->used == 0) {
      g->nodes[index] = NODE_TYPE_EMPTY;
      g->initialEmptyPos = (position){n->x, n->y};
    } else if (n->used > 400) {
      g->nodes[index] = NODE_TYPE_FULL;
    } else {
      g->nodes[index] = NODE_TYPE_REGULAR;
    }
  }
}

static void skip_whitespaces(char *line, char **output) {
  while (*line && isspace(*line))
    line++;
  *output = line;
}

static void parse_line(char *line, size_t length, void *userData) {
  if (*line == '/') {
    node n = {0};
    n.x = strtol(line + 16, &line, 10);
    n.y = strtol(line + 2, &line, 10);
    skip_whitespaces(line, &line);
    n.size = strtol(line, &line, 10);
    skip_whitespaces(line + 1, &line);
    n.used = strtol(line, NULL, 10);
    parsing_context *const ctx = userData;
    AuxArrayNodePush(&ctx->nodes, n);
    if (ctx->width < n.x)
      ctx->width = n.x;
    if (ctx->height < n.y)
      ctx->height = n.y;
  }
}

static uint32_t solve_part1(const grid *const g) {
  uint32_t viablePairs = 0;
  const uint8_t lookup[NODE_TYPE_COUNT] = {
      [NODE_TYPE_REGULAR] = 1,
      [NODE_TYPE_DATA] = 1,
  };
  const size_t size = g->width * g->height;
  for (size_t i = 0; i < size; ++i)
    viablePairs += lookup[g->nodes[i]];
  return viablePairs;
}

static void get_next_positions(const grid *const g,
                               const position *const current,
                               position *nextPositions, uint32_t *count) {
  *count = 0;
  // left
  if (current->x > 0 &&
      g->nodes[current->y * g->width + current->x - 1] != NODE_TYPE_FULL) {
    nextPositions[*count] = (position){.x = current->x - 1, .y = current->y};
    (*count)++;
  }
  // right
  if (current->x < g->width - 1 &&
      g->nodes[current->y * g->width + current->x + 1] != NODE_TYPE_FULL) {
    nextPositions[*count] = (position){.x = current->x + 1, .y = current->y};
    (*count)++;
  }
  // up
  if (current->y > 0 &&
      g->nodes[(current->y - 1) * g->width + current->x] != NODE_TYPE_FULL) {
    nextPositions[*count] = (position){.x = current->x, .y = current->y - 1};
    (*count)++;
  }
  // down
  if (current->y < g->height - 1 &&
      g->nodes[(current->y + 1) * g->width + current->x] != NODE_TYPE_FULL) {
    nextPositions[*count] = (position){.x = current->x, .y = current->y + 1};
    (*count)++;
  }
}

static uint32_t shortest_path(const grid *const g, const position from,
                              const position to) {
  AuxHashsetPos visited = {0};
  AuxHashsetPosCreate(&visited, 2048);

  AuxArrayPos positions = {0};
  AuxArrayPosCreate(&positions, 128);

  AuxArrayPosPush(&positions, from);
  AuxHashsetPosInsert(&visited, from);

  position nextPositions[4] = {0};
  uint32_t nextPositionsCount = 0;

  uint32_t distance = 0;
  while (positions.length > 0) {
    const size_t length = positions.length;
    for (size_t i = 0; i < length; ++i) {
      const position *const current = &positions.items[i];

      get_next_positions(g, current, nextPositions, &nextPositionsCount);
      for (uint32_t j = 0; j < nextPositionsCount; ++j) {
        if (position_equals(&nextPositions[j], &to)) {
          distance++;
          goto finish;
        }
        uint32_t hash;
        if (!AuxHashsetPosContains(&visited, nextPositions[j], &hash)) {
          AuxHashsetPosInsertPreHashed(&visited, nextPositions[j], hash);
          AuxArrayPosPush(&positions, nextPositions[j]);
        }
      }
    }

    const size_t newLength = positions.length - length;
    for (size_t i = 0; i < newLength; ++i) {
      positions.items[i] = positions.items[length + i];
    }
    positions.length = newLength;
    distance++;
  }

finish:
  AuxArrayPosDestroy(&positions);
  AuxHashsetPosDestroy(&visited);
  return distance;
}

static uint32_t solve_part2(const grid *const g) {
  uint32_t solution = 0;

  // first find the shortest path to move the empty space left of G
  // so it looks like this:
  // . _ G
  // . . .
  // . . .
  solution += shortest_path(g, g->initialEmptyPos,
                            (position){.x = g->initialDataPos.x - 1, .y = 0});

  // +1 to move Data (G) to the empty slot
  solution++;

  // to move G a single place to the left it takes 5 steps
  // . _ G    . G _    . G .    . G .    . G .    _ G .    G _ .
  // . . . => . . . => . . _ => . _ . => _ . . => . . . => . . .
  // . . .    . . .    . . .    . . .    . . .    . . .    . . .
  // therefore it takes Distance between destination and G times 5
  solution += (5 * (g->initialDataPos.x - 1));
  return solution;
}

int main(void) {
  parsing_context ctx = {0};
  AuxArrayNodeCreate(&ctx.nodes, 1080);
  AuxReadFileLineByLine("day22/input.txt", parse_line, &ctx);
  ctx.width++;
  ctx.height++;

  grid g = {0};
  grid_init(&ctx, &g);
  AuxArrayNodeDestroy(&ctx.nodes);

  const uint32_t part1 = solve_part1(&g);
  const uint32_t part2 = solve_part2(&g);

  printf("%u\n", part1);
  printf("%u\n", part2);
  free(g.nodes);
}
