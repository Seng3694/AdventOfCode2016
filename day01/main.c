#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

#include <aux.h>

typedef enum {
  TURN_DIR_LEFT,
  TURN_DIR_RIGHT,
} turn_direction;

typedef enum {
  FACE_DIR_NORTH,
  FACE_DIR_EAST,
  FACE_DIR_SOUTH,
  FACE_DIR_WEST,
} face_direction;

typedef struct {
  turn_direction direction;
  int length;
} instruction;

#define ARRAY_T instruction
#define ARRAY_T_NAME Instr
#include <aux_array.h>

typedef struct {
  int x;
  int y;
} position;

static const position MOVE_POSITIONS[] = {
    [FACE_DIR_NORTH] = {0, -1},
    [FACE_DIR_EAST] = {1, 0},
    [FACE_DIR_SOUTH] = {0, 1},
    [FACE_DIR_WEST] = {-1, 0},
};

static void parse(char *input, AuxArrayInstr *instructions) {
  AuxArrayInstrCreate(instructions, 128);
  while (*input != '\0') {
    instruction instr = {0};
    instr.direction = *input == 'L' ? TURN_DIR_LEFT : TURN_DIR_RIGHT;
    input++;
    instr.length = (int)strtol(input, &input, 10);
    while (*input != '\0' && (*input == ' ' || *input == ',')) {
      input++;
    }
    AuxArrayInstrPush(instructions, instr);
  }
}

static inline int wrap(const int value, const int min, const int max) {
  if (value < min)
    return max;
  else if (value > max)
    return min;
  else
    return value;
}

static inline int distance(const position pos) {
  return abs(pos.x) + abs(pos.y);
}

static inline void move(position *pos, const face_direction direction,
                        const int amount) {
  pos->x += MOVE_POSITIONS[direction].x * amount;
  pos->y += MOVE_POSITIONS[direction].y * amount;
}

static int solve_part1(const AuxArrayInstr *const instructions) {
  position pos = {0, 0};
  face_direction dir = FACE_DIR_NORTH;
  for (size_t i = 0; i < instructions->length; ++i) {
    const instruction *current = &instructions->items[i];
    const int change = current->direction == TURN_DIR_RIGHT ? 1 : -1;
    dir = wrap(dir + change, 0, 3);
    move(&pos, dir, current->length);
  }
  return distance(pos);
}

static inline uint32_t hash_position(const position pos) {
  uint32_t hash = 2166136261u;
  hash ^= pos.x;
  hash *= 16777619;
  hash ^= pos.y;
  hash *= 16777619;
  return hash;
}

static int solve_part2(const AuxArrayInstr *const instructions) {
  position pos = {0, 0};
  face_direction dir = FACE_DIR_NORTH;
  // sum up lengths to know how many points there can be
  size_t positionsCapacity = 0;
  for (size_t i = 0; i < instructions->length; ++i) {
    positionsCapacity += instructions->items[i].length;
  }
  // add 50% extra so probing won't take too long to find empty spots
  positionsCapacity *= 1.5;
  // sentinel value
  const uint32_t unset = 0xdeadb0b1;
  uint32_t *positions = malloc(sizeof(uint32_t) * positionsCapacity);
  for (size_t i = 0; i < positionsCapacity; ++i) {
    positions[i] = unset;
  }

  uint32_t hash = hash_position(pos);
  size_t index = hash % positionsCapacity;
  positions[index] = hash;

  for (size_t i = 0; i < instructions->length; ++i) {
    const instruction *current = &instructions->items[i];
    const int change = current->direction == TURN_DIR_RIGHT ? 1 : -1;
    dir = wrap(dir + change, 0, 3);

    for (size_t j = 0; j < current->length; ++j) {
      move(&pos, dir, 1);
      hash = hash_position(pos);
      index = hash % positionsCapacity;
      // probe for free spot
      while (positions[index] != unset && positions[index] != hash) {
        index = (index + 1) % positionsCapacity;
      }
      if (positions[index] == hash) {
        goto done;
      }
      positions[index] = hash;
    }
  }

done:
  free(positions);
  return distance(pos);
}

int main(void) {
  char *input = NULL;
  size_t length = 0;
  if (!AuxReadFileToString("day01/input.txt", &input, &length)) {
    return EXIT_FAILURE;
  }

  AuxArrayInstr instructions;
  parse(input, &instructions);

  const int part1 = solve_part1(&instructions);
  const int part2 = solve_part2(&instructions);

  printf("%d\n", part1);
  printf("%d\n", part2);

  AuxArrayInstrDestroy(&instructions);
  free(input);
}
