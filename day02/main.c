#include <stdio.h>
#include <string.h>
#include <aux.h>

typedef struct {
  int8_t x;
  int8_t y;
} position;

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
    printf("pos: %d %d\n", pos.x, pos.y);
    AuxArrayCharPush(&code, '1' + (pos.y * 3 + pos.x));
  }
  const int solution = (int)strtol(code.items, NULL, 10);
  AuxArrayCharDestroy(&code);
  return solution;
}

int main(void) {
  AuxArrayString instructions;
  AuxArrayStringCreate(&instructions, 8);
  AuxReadFileLineByLine("day02/input.txt", parse_line, &instructions);

  const int part1 = solve_part1(&instructions);

  printf("%d\n", part1);

  for (size_t i = 0; i < instructions.length; ++i) {
    AuxArrayCharDestroy(&instructions.items[i]);
  }
  AuxArrayStringDestroy(&instructions);
}
