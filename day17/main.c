#include <aoc/aoc.h>
#include <aoc/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT,
} direction;

typedef struct {
  uint8_t x;
  uint8_t y;
} position;

static const char directionChars[] = {'U', 'D', 'L', 'R'};
static const position directionMoves[] = {
    {0, -1}, // UP
    {0, 1},  // DOWN
    {-1, 0}, // LEFT
    {1, 0},  // RIGHT
};

#define AOC_T char
#define AOC_T_NAME Dir
#include <aoc/array.h>

typedef struct {
  position pos;
  AocArrayDir path;
} state;

#define AOC_T state
#define AOC_T_NAME State
#include <aoc/array.h>

static const char hex_lookup[] = "0123456789abcdef";

static inline char get_hash_char(const uint8_t *const hash,
                                 const uint8_t digit) {
  const uint8_t subIndex = 4 * (1 - (digit & 1));
  return hex_lookup[(hash[digit >> 1] & (0x0f << subIndex)) >> subIndex];
}

state clone_state(const state *const s) {
  state clone = {.pos = s->pos};
  AocArrayDirCreate(&clone.path, s->path.capacity);
  clone.path.length = s->path.length;
  memcpy(clone.path.items, s->path.items, sizeof(char) * s->path.capacity);
  return clone;
}

static inline bool can_move_up(const uint8_t *const hash, const position pos) {
  return pos.y > 0 && get_hash_char(hash, 0) > 'a';
}

static inline bool can_move_down(const uint8_t *const hash,
                                 const position pos) {
  return pos.y < 3 && get_hash_char(hash, 1) > 'a';
}

static inline bool can_move_left(const uint8_t *const hash,
                                 const position pos) {
  return pos.x > 0 && get_hash_char(hash, 2) > 'a';
}

static inline bool can_move_right(const uint8_t *const hash,
                                  const position pos) {
  return pos.x < 3 && get_hash_char(hash, 3) > 'a';
}

static inline void move(position *const pos, const position dist) {
  pos->x += dist.x;
  pos->y += dist.y;
}

static void append_move(AocArrayState *states, const state *const current,
                        const direction dir) {
  state clone = clone_state(current);
  AocArrayDirPush(&clone.path, directionChars[dir]);
  move(&clone.pos, directionMoves[dir]);
  AocArrayStatePush(states, clone);
}

static inline bool has_reached_vault(const uint8_t x, const uint8_t y) {
  return x == 3 && y == 3;
}

static void solve(const char *const input, const size_t inputLength,
                  const bool part1) {
  AocArrayState states = {0};
  AocArrayStateCreate(&states, 128);

  state start = {.pos = {0, 0}};
  AocArrayDirCreate(&start.path, 64);
  memset(start.path.items, 0, sizeof(char) * start.path.capacity);

  AocArrayStatePush(&states, start);

  size_t hashBufferCapacity = 128;
  char *hashBuffer = malloc(sizeof(char) * hashBufferCapacity);
  memcpy(hashBuffer, input, sizeof(char) * inputLength);

  uint8_t hash[16] = {0};
  size_t longestPath = 0;

  while (states.length > 0) {
    const size_t statesLength = states.length;
    for (size_t i = 0; i < statesLength; ++i) {
      // for convenience. Can't store it in a variable because "items" might be
      // reallocated
#define CURRENT states.items[i]

      if (CURRENT.path.length + inputLength > hashBufferCapacity) {
        hashBufferCapacity *= 2;
        hashBuffer = realloc(hashBuffer, hashBufferCapacity);
      }

      memcpy(hashBuffer + inputLength, CURRENT.path.items,
             sizeof(char) * CURRENT.path.length);
      AocMD5(hashBuffer, inputLength + CURRENT.path.length, hash);

      // moves will only be appended when they don't reach the vault.
      if (can_move_down(hash, CURRENT.pos)) {
        if (has_reached_vault(CURRENT.pos.x, CURRENT.pos.y + 1)) {
          if (part1) {
            printf("%.*s\n", (int)CURRENT.path.length, CURRENT.path.items);
            goto finish;
          }
          if (longestPath < CURRENT.path.length) {
            longestPath = CURRENT.path.length + 1;
          }
        } else {
          append_move(&states, &CURRENT, DIRECTION_DOWN);
        }
      }
      if (can_move_right(hash, CURRENT.pos)) {
        if (has_reached_vault(CURRENT.pos.x + 1, CURRENT.pos.y)) {
          if (part1) {
            printf("%.*s\n", (int)CURRENT.path.length, CURRENT.path.items);
            goto finish;
          }
          if (longestPath < CURRENT.path.length) {
            longestPath = CURRENT.path.length + 1;
          }
        } else {
          append_move(&states, &CURRENT, DIRECTION_RIGHT);
        }
      }
      // UP and LEFT can't reach the destination so no check there
      if (can_move_up(hash, CURRENT.pos)) {
        append_move(&states, &CURRENT, DIRECTION_UP);
      }
      if (can_move_left(hash, CURRENT.pos)) {
        append_move(&states, &CURRENT, DIRECTION_LEFT);
      }
    }
#undef CURRENT

    for (size_t i = 0; i < statesLength; ++i) {
      // TODO: think of a smarter way of reusing them
      AocArrayDirDestroy(&states.items[i].path);
    }

    const size_t newLength = states.length - statesLength;
    for (size_t i = 0; i < newLength; ++i) {
      states.items[i] = states.items[statesLength + i];
    }
    states.length = newLength;
  }

  // part 2
  printf("%zu\n", longestPath);

finish:
  free(hashBuffer);
  for (size_t i = 0; i < states.length; ++i)
    AocArrayDirDestroy(&states.items[i].path);
  AocArrayStateDestroy(&states);
}

int main(void) {
  const char input[] = "ioramepc";

  solve(input, sizeof(input) - 1, true);
  solve(input, sizeof(input) - 1, false);
}
