#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static uint32_t highest_bit(const uint32_t n) {
  return 1 << (uint32_t)log2((double)n);
}

static uint32_t solve_part1(const uint32_t n) {
  // Josephus Problem: https://www.youtube.com/watch?v=uCsD3ZGzMgE
  // move leading bit to the end
  return (n ^ highest_bit(n)) << 1 | 1;
}

typedef struct node {
  struct node *left;
  struct node *right;
} node;

static void remove_node(node *n, uint32_t offset) {
  while (offset > 0) {
    offset--;
    n = n->left;
  }
  n->left->right = n->right;
  n->right->left = n->left;
  n->left = NULL;
  n->right = NULL;
}

static uint32_t solve_part2(const uint32_t n) {
  node *nodes = malloc(sizeof(node) * n);
  nodes[0].left = &nodes[1];
  nodes[0].right = &nodes[n - 1];
  nodes[n - 1].left = &nodes[0];
  nodes[n - 1].right = &nodes[n - 2];
  for (size_t i = 1; i < n - 1; ++i) {
    nodes[i].left = &nodes[i + 1];
    nodes[i].right = &nodes[i - 1];
  }

#ifndef NDEBUG
#define PRINT_NODES()                                                          \
  do {                                                                         \
    for (uint32_t i = 0; i < n; ++i)                                           \
      putc(nodes[i].left ? '#' : '.', stdout);                                 \
    putc('\n', stdout);                                                        \
  } while (0)
#else
#define PRINT_NODES()
#endif

  PRINT_NODES();
  uint32_t remaining = n;
  node *current = &nodes[0];
  while (remaining > 1) {
    uint32_t next = remaining >> 1;
    remove_node(current, next);
    current = current->left;
    remaining--;
    PRINT_NODES();
  }

#undef PRINT_NODES
  const uint32_t solution = current - nodes;
  free(nodes);
  return solution + 1;
}

int main(void) {
  const uint32_t input = 3004953;
  const uint32_t part1 = solve_part1(input);
  const uint32_t part2 = solve_part2(input);
  printf("%u\n", part1);
  printf("%u\n", part2);
}
