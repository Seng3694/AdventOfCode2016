#include <aux.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
  uint16_t size;
  uint16_t used;
  uint8_t x;
  uint8_t y;
} node;

static int compare_node(const void *const a, const void *const b) {
  const node *const na = a;
  const node *const nb = b;
  int ia = na->y * INT16_MAX + na->x;
  int ib = nb->y * INT16_MAX + nb->x;
  return ia - ib;
}

#define AUX_T node
#define AUX_T_NAME Node
#include <aux_array.h>

typedef struct {
  uint8_t width;
  uint8_t height;
  AuxArrayNode nodes;
} context;

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
    context *const ctx = userData;
    AuxArrayNodePush(&ctx->nodes, n);
    if (ctx->width < n.x)
      ctx->width = n.x;
    if (ctx->height < n.y)
      ctx->height = n.y;
  }
}

static uint32_t solve_part1(const context *const ctx) {
  uint32_t viablePairs = 0;
  for (size_t i = 0; i < ctx->nodes.length; ++i) {
    const node *const n1 = &ctx->nodes.items[i];
    if (n1->used == 0)
      continue;
    for (size_t j = 0; j < ctx->nodes.length; ++j) {
      if (i == j)
        continue;
      const node *const n2 = &ctx->nodes.items[j];
      if (n2->size - n2->used >= n1->used)
        viablePairs++;
    }
  }
  return viablePairs;
}

int main(void) {
  context ctx = {0};
  AuxArrayNodeCreate(&ctx.nodes, 1080);
  AuxReadFileLineByLine("day22/input.txt", parse_line, &ctx);
  ctx.width++;
  ctx.height++;
  qsort(ctx.nodes.items, ctx.nodes.length, sizeof(node), compare_node);

  const uint32_t part1 = solve_part1(&ctx);

  printf("%u\n", part1);

  AuxArrayNodeDestroy(&ctx.nodes);
}
