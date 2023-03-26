#include <stdio.h>
#include <string.h>
#include <aux.h>

typedef enum {
  MOVE_DIRECTION_UP,
  MOVE_DIRECTION_RIGHT,
  MOVE_DIRECTION_DOWN,
  MOVE_DIRECTION_LEFT,
  MOVE_DIRECTION_COUNT,
} move_direction;

static const char *index_to_char_mapping = "123456789ABCD";

static const int8_t keypad1_move_table[MOVE_DIRECTION_COUNT][0xD] = {
    {0, 0, 0, -3, -3, -3, -3, -3, -3}, // UP
    {1, 1, 0, 1, 1, 0, 1, 1, 0},       // RIGHT
    {3, 3, 3, 3, 3, 3, 0, 0, 0},       // DOWN
    {0, -1, -1, 0, -1, -1, 0, -1, -1}, // LEFT
};

static const int8_t keypad2_move_table[MOVE_DIRECTION_COUNT][0xD] = {
    {0, 0, -2, 0, 0, -4, -4, -4, 0, -4, -4, -4, -2}, // UP
    {0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0},         // RIGHT
    {2, 4, 4, 4, 0, 4, 4, 4, 0, 0, 2, 0, 0},         // DOWN
    {0, 0, -1, -1, 0, -1, -1, -1, -1, 0, -1, -1, 0}, // LEFT
};

#define AUX_T char
#define AUX_T_NAME Char
#include <aux_array.h>

#define AUX_T AuxArrayChar
#define AUX_T_NAME String
#include <aux_array.h>

void parse_line(char *line, size_t length, void *userData) {
  AuxArrayString *arr = (AuxArrayString *)userData;
  AuxArrayChar str;
  AuxRemoveTrailingWhitespace(line, &length);
  AuxArrayCharCreate(&str, length + 1);
  memcpy(str.items, line, length + 1);
  str.length = length;
  AuxArrayStringPush(arr, str);
}

static inline move_direction char_to_move_direction(const char c) {
  switch (c) {
  case 'R':
    return MOVE_DIRECTION_RIGHT;
  case 'D':
    return MOVE_DIRECTION_DOWN;
  case 'L':
    return MOVE_DIRECTION_LEFT;
  default: //'U'
    return MOVE_DIRECTION_UP;
  }
}

static AuxArrayChar solve(const AuxArrayString *const instructions,
                          const int8_t moveTable[MOVE_DIRECTION_COUNT][0xD]) {
  AuxArrayChar code;
  AuxArrayCharCreate(&code, 32);
  memset(code.items, 0, code.capacity);
  int8_t index = 4;
  for (size_t i = 0; i < instructions->length; ++i) {
    const AuxArrayChar *const directions = &instructions->items[i];
    for (size_t j = 0; j < directions->length; ++j) {
      const move_direction dir = char_to_move_direction(directions->items[j]);
      index += moveTable[dir][index];
    }
    AuxArrayCharPush(&code, index_to_char_mapping[index]);
  }
  return code;
}

int main(void) {
  AuxArrayString instructions;
  AuxArrayStringCreate(&instructions, 8);
  AuxReadFileLineByLine("day02/input.txt", parse_line, &instructions);

  AuxArrayChar part1 = solve(&instructions, keypad1_move_table);
  AuxArrayChar part2 = solve(&instructions, keypad2_move_table);

  printf("%s\n", part1.items);
  printf("%s\n", part2.items);

  AuxArrayCharDestroy(&part1);
  AuxArrayCharDestroy(&part2);

  for (size_t i = 0; i < instructions.length; ++i) {
    AuxArrayCharDestroy(&instructions.items[i]);
  }
  AuxArrayStringDestroy(&instructions);
}
