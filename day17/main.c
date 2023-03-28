#include <aux.h>
#include <aux_md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  uint8_t x;
  uint8_t y;
} position;

#define AUX_T char
#define AUX_T_NAME Dir
#include <aux_array.h>

typedef struct {
  position pos;
  AuxArrayDir path;
} state;

#define AUX_T state
#define AUX_T_NAME State
#include <aux_array.h>

static const char hex_lookup[] = "0123456789abcdef";

static inline char get_hash_char(const uint8_t *const hash,
                                 const uint8_t digit) {
  const uint8_t subIndex = 4 * (1 - (digit & 1));
  return hex_lookup[(hash[digit >> 1] & (0x0f << subIndex)) >> subIndex];
}

state clone_state(const state *const s) {
  state clone = {.pos = s->pos};
  AuxArrayDirCreate(&clone.path, s->path.capacity);
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

static void solve(const char *const input, const size_t inputLength,
                  const bool part1) {
  AuxArrayState states = {0};
  AuxArrayStateCreate(&states, 128);

  state start = {.pos = {0, 0}};
  AuxArrayDirCreate(&start.path, 64);
  memset(start.path.items, 0, sizeof(char) * start.path.capacity);

  AuxArrayStatePush(&states, start);

  size_t hashBufferCapacity = 128;
  char *hashBuffer = malloc(sizeof(char) * hashBufferCapacity);
  memcpy(hashBuffer, input, sizeof(char) * inputLength);

  uint8_t hash[16] = {0};
  size_t longestPath = 0;

  while (states.length > 0) {
    const size_t statesLength = states.length;
    for (size_t i = 0; i < statesLength; ++i) {
      const state *s = &states.items[i];

      if (s->path.length + inputLength > hashBufferCapacity) {
        hashBufferCapacity *= 2;
        hashBuffer = realloc(hashBuffer, hashBufferCapacity);
      }

      memcpy(hashBuffer + inputLength, s->path.items,
             sizeof(char) * s->path.length);
      AuxMD5(hashBuffer, inputLength + s->path.length, hash);

      if (can_move_down(hash, s->pos)) {
        if (s->pos.x == 3 && s->pos.y + 1 == 3) {
          if (part1) {
            printf("%.*s\n", (int)s->path.length, s->path.items);
            goto finish;
          }
          if (longestPath < s->path.length) {
            longestPath = s->path.length + 1;
          }
        } else {
          state clone = clone_state(s);
          AuxArrayDirPush(&clone.path, 'D');
          clone.pos.y++;
          AuxArrayStatePush(&states, clone);
          s = &states.items[i];
        }
      }
      if (can_move_right(hash, s->pos)) {
        if (s->pos.x + 1 == 3 && s->pos.y == 3) {
          if (part1) {
            printf("%.*s\n", (int)s->path.length, s->path.items);
            goto finish;
          }
          if (longestPath < s->path.length) {
            longestPath = s->path.length + 1;
          }
        } else {
          state clone = clone_state(s);
          AuxArrayDirPush(&clone.path, 'R');
          clone.pos.x++;
          AuxArrayStatePush(&states, clone);
          s = &states.items[i];
        }
      }
      if (can_move_up(hash, s->pos)) {
        state clone = clone_state(s);
        AuxArrayDirPush(&clone.path, 'U');
        clone.pos.y--;
        AuxArrayStatePush(&states, clone);
        s = &states.items[i];
      }
      if (can_move_left(hash, s->pos)) {
        state clone = clone_state(s);
        AuxArrayDirPush(&clone.path, 'L');
        clone.pos.x--;
        AuxArrayStatePush(&states, clone);
        s = &states.items[i];
      }
    }

    for (size_t i = 0; i < statesLength; ++i) {
      // TODO: think of a smarter way of reusing them
      AuxArrayDirDestroy(&states.items[i].path);
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
  for (size_t i = 0; i < states.length; ++i) {
    AuxArrayDirDestroy(&states.items[i].path);
  }
  AuxArrayStateDestroy(&states);
}

int main(void) {
  const char input[] = "ioramepc";

  solve(input, sizeof(input) - 1, true);
  solve(input, sizeof(input) - 1, false);
}
