#include <stdio.h>
#include <string.h>
#include <aux.h>

#define HAS_FLAG(x, flag) (((x) & (flag)) == flag)

typedef struct {
  int8_t x;
  int8_t y;
} position;

typedef enum {
  MOVE_DIRECTION_UP,
  MOVE_DIRECTION_RIGHT,
  MOVE_DIRECTION_DOWN,
  MOVE_DIRECTION_LEFT,
  MOVE_DIRECTION_COUNT,
} move_direction;

static const char *index_to_char_mapping = "123456789ABCD";

static const int8_t keypad2_move_table[MOVE_DIRECTION_COUNT][0xD] = {
    {
        // UP
        0,  // 1
        0,  // 2
        -2, // 3
        0,  // 4
        0,  // 5
        -4, // 6
        -4, // 7
        -4, // 8
        0,  // 9
        -4, // A
        -4, // B
        -4, // C
        -2, // D
    },
    {
        // RIGHT
        0, // 1
        1, // 2
        1, // 3
        0, // 4
        1, // 5
        1, // 6
        1, // 7
        1, // 8
        0, // 9
        1, // A
        1, // B
        0, // C
        0, // D
    },
    {
        // DOWN
        2, // 1
        4, // 2
        4, // 3
        4, // 4
        0, // 5
        4, // 6
        4, // 7
        4, // 8
        0, // 9
        0, // A
        2, // B
        0, // C
        0, // D
    },
    {
        // LEFT
        0,  // 1
        0,  // 2
        -1, // 3
        -1, // 4
        0,  // 5
        -1, // 6
        -1, // 7
        -1, // 8
        -1, // 9
        0,  // A
        -1, // B
        -1, // C
        0,  // D
    },
};

#define ARRAY_T char
#define ARRAY_T_NAME Char
#include <aux_array.h>

#define ARRAY_T AuxArrayChar
#define ARRAY_T_NAME String
#include <aux_array.h>

void parse_line(char *line, size_t length, void *userData) {
  AuxArrayString *arr = (AuxArrayString *)userData;
  AuxArrayChar str;
  AuxArrayCharCreate(&str, length + 1);
  memcpy(str.items, line, length + 1);
  str.length = length;
  AuxArrayStringPush(arr, str);
}

static inline int8_t clamp(const int8_t value, const int8_t min,
                           const int8_t max) {
  if (value < min)
    return min;
  else if (value > max)
    return max;
  else
    return value;
}

static int solve_part1(const AuxArrayString *const instructions) {
  AuxArrayChar code;
  AuxArrayCharCreate(&code, 32);
  memset(code.items, 0, code.capacity);

  // 1 2 3
  // 4 5 6
  // 7 8 9
  position pos = {1, 1}; // start at 5

  for (size_t i = 0; i < instructions->length; ++i) {
    const AuxArrayChar *const directions = &instructions->items[i];
    for (size_t j = 0; j < directions->length; ++j) {
      switch (directions->items[j]) {
      case 'U':
        pos.y = clamp(pos.y - 1, 0, 2);
        break;
      case 'R':
        pos.x = clamp(pos.x + 1, 0, 2);
        break;
      case 'D':
        pos.y = clamp(pos.y + 1, 0, 2);
        break;
      case 'L':
        pos.x = clamp(pos.x - 1, 0, 2);
        break;
      }
    }
    AuxArrayCharPush(&code, '1' + (pos.y * 3 + pos.x));
  }
  const int solution = (int)strtol(code.items, NULL, 10);
  AuxArrayCharDestroy(&code);
  return solution;
}

static AuxArrayChar solve_part2(const AuxArrayString *const instructions) {
  AuxArrayChar code;
  AuxArrayCharCreate(&code, 32);
  memset(code.items, 0, code.capacity);
  int8_t index = 4;
  for (size_t i = 0; i < instructions->length; ++i) {
    const AuxArrayChar *const directions = &instructions->items[i];

    for (size_t j = 0; j < directions->length; ++j) {
      switch (directions->items[j]) {
      case 'U':
        index += keypad2_move_table[MOVE_DIRECTION_UP][index];
        break;
      case 'R':
        index += keypad2_move_table[MOVE_DIRECTION_RIGHT][index];
        break;
      case 'D':
        index += keypad2_move_table[MOVE_DIRECTION_DOWN][index];
        break;
      case 'L':
        index += keypad2_move_table[MOVE_DIRECTION_LEFT][index];
        break;
      }
    }
    AuxArrayCharPush(&code, index_to_char_mapping[index]);
  }
  return code;
}

int main(void) {
  AuxArrayString instructions;
  AuxArrayStringCreate(&instructions, 8);
  AuxReadFileLineByLine("day02/input.txt", parse_line, &instructions);

  const int part1 = solve_part1(&instructions);
  AuxArrayChar part2 = solve_part2(&instructions);

  printf("%d\n", part1);
  printf("%s\n", part2.items);

  AuxArrayCharDestroy(&part2);

  for (size_t i = 0; i < instructions.length; ++i) {
    AuxArrayCharDestroy(&instructions.items[i]);
  }
  AuxArrayStringDestroy(&instructions);
}
