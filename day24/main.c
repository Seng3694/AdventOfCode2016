#include <aux.h>
#include <ctype.h>
#include <math.h>

#define AUX_T char
#define AUX_T_NAME Char
#include <aux_array.h>

#define AUX_T uint32_t
#define AUX_T_NAME Uint32
#include <aux_array.h>

#define TILE_WALL '#'
#define TILE_EMPTY '.'

typedef struct {
  int16_t x;
  int16_t y;
} position;

typedef struct {
  position pois[10];
  uint8_t poiCount;
  uint8_t width;
  uint8_t height;
  AuxArrayChar data;
} map;

typedef struct {
  uint32_t lineCount;
  map *const m;
} parsing_context;

static void parse_line(char *line, size_t length, void *userData) {
  AuxRemoveTrailingWhitespace(line, &length);
  parsing_context *ctx = userData;
  ctx->m->width = length;
  for (size_t i = 0; i < length; ++i) {
    if (isdigit(line[i])) {
      ctx->m->pois[line[i] - '0'] = (position){
          .x = (int16_t)i,
          .y = (int16_t)ctx->lineCount,
      };
      ctx->m->poiCount++;
    }
    AuxArrayCharPush(&ctx->m->data, line[i]);
  }
  ctx->lineCount++;
}

static inline uint32_t taxicab_distance(const position from,
                                        const position to) {
  return (uint32_t)abs((int8_t)from.x - (int8_t)to.x) +
         (uint32_t)abs((int8_t)from.y - (int8_t)to.y);
}

typedef struct {
  position pos;
  uint32_t length;
  int gCost;
  int hCost;
} astar_node;

static int astar_node_compare(const astar_node *const a,
                              const astar_node *const b) {
  const int fCostDiff = (a->gCost + a->hCost) - (b->gCost + b->hCost);
  return fCostDiff == 0 ? a->hCost - b->hCost : fCostDiff;
}

static bool astar_node_equals(const astar_node *const a,
                              const astar_node *const b) {
  return a->pos.x == b->pos.x && a->pos.y == b->pos.y;
}

static uint32_t astar_node_hash(const astar_node *const node) {
  return 2166136261u ^ ((uint32_t)node->pos.x * 16777619) ^
         ((uint32_t)node->pos.y * 57620617);
}

#define AUX_T astar_node
#define AUX_T_NAME AStarNode
#define AUX_T_EMPTY ((astar_node){0})
#define AUX_T_HFUNC(x) astar_node_hash(x)
#define AUX_T_EQUALS(a, b) astar_node_equals(a, b)
#include <aux_hashset.h>

#define AUX_T astar_node
#define AUX_T_NAME AStarNode
#define AUX_T_COMPARE(a, b) astar_node_compare(a, b)
#include <aux_heap.h>

static void get_neighbors(const astar_node current, const map *const m,
                          astar_node neighbors[const 4],
                          size_t *const neigborCount) {
  *neigborCount = 0;
  const position nPos[4] = {
      {current.pos.x - 1, current.pos.y},
      {current.pos.x + 1, current.pos.y},
      {current.pos.x, current.pos.y - 1},
      {current.pos.x, current.pos.y + 1},
  };
  for (size_t i = 0; i < 4; ++i) {
    if (nPos[i].x < 0 || nPos[i].x >= m->width || nPos[i].y < 0 ||
        nPos[i].y >= m->height)
      continue;

    const size_t index = nPos[i].y * m->width + nPos[i].x;
    if (m->data.items[index] != TILE_WALL) {
      neighbors[(*neigborCount)++] = (astar_node){.pos = nPos[i]};
    }
  }
}

static uint32_t calculate_path_length(const map *const m, const uint8_t from,
                                      const uint8_t to) {
  const position start = m->pois[from];
  const position dest = m->pois[to];
  uint32_t solution = 0;

  AuxMinHeapAStarNode heap = {0};
  AuxMinHeapAStarNodeCreate(&heap, 128);

  AuxHashsetAStarNode open = {0};
  AuxHashsetAStarNodeCreate(&open, 128);

  AuxHashsetAStarNode closed = {0};
  AuxHashsetAStarNodeCreate(&closed, 128);

  const astar_node s = {
      .pos = start,
      .gCost = 0,
      .hCost = taxicab_distance(start, dest) * 10,
  };
  AuxMinHeapAStarNodePush(&heap, s);
  AuxHashsetAStarNodeInsert(&open, s);

  astar_node neighborBuffer[4] = {0};
  size_t neighborCount = 0;

  while (heap.count > 0) {
    const astar_node current = AuxMinHeapAStarNodePop(&heap);
    const uint32_t hash = astar_node_hash(&current);
    AuxHashsetAStarNodeRemovePreHashed(&open, current, hash);
    AuxHashsetAStarNodeInsertPreHashed(&closed, current, hash);

    if (current.pos.x == dest.x && current.pos.y == dest.y) {
      solution = current.length;
      goto finish;
    }

    get_neighbors(current, m, neighborBuffer, &neighborCount);
    for (size_t i = 0; i < neighborCount; ++i) {
      if (AuxHashsetAStarNodeContains(&closed, neighborBuffer[i], NULL))
        continue;

      const uint32_t newCost =
          current.gCost +
          taxicab_distance(current.pos, neighborBuffer[i].pos) * 10;

      uint32_t hash = 0;
      const bool isNotInOpen =
          !AuxHashsetAStarNodeContains(&open, neighborBuffer[i], &hash);
      if (newCost < neighborBuffer[i].gCost || isNotInOpen) {
        neighborBuffer[i].gCost = newCost;
        neighborBuffer[i].hCost =
            taxicab_distance(dest, neighborBuffer[i].pos) * 10;
        neighborBuffer[i].length = current.length + 1;

        if (isNotInOpen) {
          AuxMinHeapAStarNodePush(&heap, neighborBuffer[i]);
          AuxHashsetAStarNodeInsertPreHashed(&open, neighborBuffer[i], hash);
        }
      }
    }
  }

finish:
  AuxHashsetAStarNodeDestroy(&open);
  AuxHashsetAStarNodeDestroy(&closed);
  AuxMinHeapAStarNodeDestroy(&heap);
  return solution;
}

typedef struct {
  const uint32_t *pathLengths[10];
} node;

typedef struct {
  uint32_t shortestPath;
  uint32_t shortestPathFull;
  size_t count;
  node *nodes;
} permutation_data;

static void evaluate_permutation(const size_t *const indices, void *userData) {
  permutation_data *data = userData;

  if (indices[1] < indices[data->count - 2]) {
    uint32_t distance = 0;
    for (size_t i = 0; i < data->count - 1; ++i) {
      const size_t nextIndex = i + 1;
      distance += *data->nodes[indices[i]].pathLengths[indices[nextIndex]];
    }

    if (distance < data->shortestPath) {
      data->shortestPath = distance;
    }

    distance += *data->nodes[indices[data->count - 1]].pathLengths[indices[0]];
    if (distance < data->shortestPathFull) {
      data->shortestPathFull = distance;
    }
  }
}

static void generate_permutations(size_t *const indices, size_t n,
                                  void *userData) {
  if (n == 1) {
    evaluate_permutation(indices, userData);
  } else {
    for (size_t i = 0; i < n; ++i) {
      generate_permutations(indices, n - 1, userData);
      const size_t swapIndex = (n % 2 == 0) ? i : 0;
      const uint32_t tmp = indices[swapIndex];
      indices[swapIndex] = indices[n - 1];
      indices[n - 1] = tmp;
    }
  }
}

static void solve_both(const map *const m, uint32_t *const part1,
                       uint32_t *const part2) {
  uint32_t count = m->poiCount * (m->poiCount - 1) / 2;

  AuxArrayUint32 pathLengths = {0};
  AuxArrayUint32Create(&pathLengths, count);
  memset(pathLengths.items, 0, sizeof(uint32_t) * count);

  node nodes[10] = {0};

  for (uint8_t from = 0; from < m->poiCount - 1; ++from) {
    for (uint8_t to = from + 1; to < m->poiCount; ++to) {
      AuxArrayUint32Push(&pathLengths, calculate_path_length(m, from, to));
      nodes[from].pathLengths[to] = AuxArrayUint32Last(&pathLengths);
      nodes[to].pathLengths[from] = AuxArrayUint32Last(&pathLengths);
    }
  }

  permutation_data data = {
      .count = m->poiCount,
      .nodes = nodes,
      .shortestPath = UINT32_MAX,
      .shortestPathFull = UINT32_MAX,
  };

  size_t indices[10] = {0, 1, 2, 3, 4, 5, 6, 7};
  generate_permutations(indices, data.count, &data);

  AuxArrayUint32Destroy(&pathLengths);

  *part1 = data.shortestPath;
  *part2 = data.shortestPathFull;
}

int main(void) {
  map m = {0};
  AuxArrayCharCreate(&m.data, 181 * 40);

  parsing_context ctx = {.m = &m};
  AuxReadFileLineByLine("day24/input.txt", parse_line, &ctx);
  m.height = ctx.lineCount;

  uint32_t part1 = 0;
  uint32_t part2 = 0;
  solve_both(&m, &part1, &part2);

  printf("%u\n", part1);
  printf("%u\n", part2);

  AuxArrayCharDestroy(&m.data);
}
