#include <aux.h>
#include <stdio.h>

typedef struct {
  uint32_t a;
  uint32_t b;
  uint32_t c;
} triangle;

#define ARRAY_T triangle
#define ARRAY_T_NAME Triangle
#include <aux_array.h>

static void parse_line(char *line, size_t length, void *userData) {
  AuxRemoveTrailingWhitespace(line, &length);
  triangle t = {0};
  sscanf(line, "%u %u %u", &t.a, &t.b, &t.c);
  AuxArrayTriangle *triangles = userData;
  AuxArrayTrianglePush(triangles, t);
}

static inline bool is_valid_triangle(const uint32_t a, const uint32_t b,
                                     const uint32_t c) {
  return a + b > c && a + c > b && b + c > a;
}

static int solve_part1(const AuxArrayTriangle *const triangles) {
  int solution = 0;
  for (size_t i = 0; i < triangles->length; ++i) {
    const triangle *const t = &triangles->items[i];
    if (is_valid_triangle(t->a, t->b, t->c))
      solution++;
  }
  return solution;
}

int main(void) {
  AuxArrayTriangle triangles;
  AuxArrayTriangleCreate(&triangles, 2000);
  AuxReadFileLineByLine("day03/input.txt", parse_line, &triangles);

  int part1 = solve_part1(&triangles);

  printf("%d\n", part1);

  AuxArrayTriangleDestroy(&triangles);
}
