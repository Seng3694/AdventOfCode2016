#include <aux.h>
#include <stdio.h>

typedef struct {
  uint32_t x;
  uint32_t y;
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
#define AUX_T_EMPTY ((position){0, 0})
#define AUX_T_HFUNC(x) calc_position_hash(x)
#define AUX_T_EQUALS(a, b) position_equals(a, b)
#include <aux_hashset.h>

#define ARRAY_T position
#define ARRAY_T_NAME Pos
#include <aux_array.h>

static inline uint32_t function(const uint32_t x, const uint32_t y) {
  return x * x + 3 * x + 2 * x * y + y + y * y;
}

static inline bool is_even(const uint32_t value) {
  return ((~value) & 1);
}

static inline bool is_open(const uint32_t x, const uint32_t y,
                           const uint32_t input) {
  return is_even(AuxPopCnt32(function(x, y) + input));
}

static void get_next_moves(const position *const pos, const uint32_t input,
                           AuxArrayPos *const nextMoves) {
  AuxArrayPosClear(nextMoves);
  if (pos->x > 0 && is_open(pos->x - 1, pos->y, input))
    AuxArrayPosPush(nextMoves, (position){pos->x - 1, pos->y}); // left
  if (pos->y > 0 && is_open(pos->x, pos->y - 1, input))
    AuxArrayPosPush(nextMoves, (position){pos->x, pos->y - 1}); // up
  if (is_open(pos->x + 1, pos->y, input))
    AuxArrayPosPush(nextMoves, (position){pos->x + 1, pos->y}); // right
  if (is_open(pos->x, pos->y + 1, input))
    AuxArrayPosPush(nextMoves, (position){pos->x, pos->y + 1}); // down
}

static void solve_both_parts(const uint32_t input, const position destination,
                             uint32_t *const part1, uint32_t *const part2) {
  AuxHashsetPos visited = {0};
  AuxHashsetPosCreate(&visited, 64);
  AuxArrayPos moves = {0};
  AuxArrayPosCreate(&moves, 64);

  const position start = {1, 1};
  AuxArrayPosPush(&moves, start);
  AuxHashsetPosInsert(&visited, start);

  AuxArrayPos nextMoves = {0};
  AuxArrayPosCreate(&nextMoves, 4);

  uint32_t distance = 0;

  while (moves.length > 0) {
    if (distance == 50)
      *part2 = visited.count;
    const size_t length = moves.length;
    for (size_t i = 0; i < length; ++i) {
      const position *const current = &moves.items[i];
      if (position_equals(current, &destination)) {
        goto finish;
      }

      get_next_moves(current, input, &nextMoves);
      for (size_t j = 0; j < nextMoves.length; ++j) {
        const position pos = nextMoves.items[j];
        uint32_t hash;
        if (!AuxHashsetPosContains(&visited, pos, &hash)) {
          AuxHashsetPosInsertPreHashed(&visited, pos, hash);
          AuxArrayPosPush(&moves, pos);
        }
      }
    }

    const size_t newLength = moves.length - length;
    for (size_t i = 0; i < newLength; ++i) {
      moves.items[i] = moves.items[length + i];
    }
    moves.length = newLength;
    distance++;
  }

finish:
  AuxArrayPosDestroy(&nextMoves);
  AuxArrayPosDestroy(&moves);
  AuxHashsetPosDestroy(&visited);

  *part1 = distance;
}

int main(void) {
  const uint32_t input = 1364;

  uint32_t part1 = 0;
  uint32_t part2 = 0;
  solve_both_parts(input, (position){31, 39}, &part1, &part2);

  printf("%u\n", part1);
  printf("%u\n", part2);
}
