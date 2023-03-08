#include <stdio.h>
#include <aux.h>
#include <aux_string.h>

typedef enum {
  MOVE_DIRECTION_UP,
  MOVE_DIRECTION_RIGHT,
  MOVE_DIRECTION_DOWN,
  MOVE_DIRECTION_LEFT,
} move_direction;

#define ARRAY_T move_direction
#define ARRAY_T_NAME Direction
#include <aux_array.h>

#define ARRAY_T AuxArrayDirection
#define ARRAY_T_NAME Instr
#include <aux_array.h>

void parse_line(char *line, size_t length, void *userData) {
  AuxArrayDirection directions;
  AuxArrayDirectionCreate(&directions, 256);

  AuxRemoveTrailingWhitespace(line, &length);
  for (size_t i = 0; i < length; ++i) {
    switch (*line) {
    case 'L':
      AuxArrayDirectionPush(&directions, MOVE_DIRECTION_LEFT);
      break;
    case 'U':
      AuxArrayDirectionPush(&directions, MOVE_DIRECTION_UP);
      break;
    case 'R':
      AuxArrayDirectionPush(&directions, MOVE_DIRECTION_RIGHT);
      break;
    case 'D':
      AuxArrayDirectionPush(&directions, MOVE_DIRECTION_DOWN);
      break;
    default:
      AUX_LOG("Invalid direction '%c' (0x%x)\n", *line, *line);
      break;
    }
  }
  AuxArrayInstr *arr = (AuxArrayInstr *)userData;
  AuxArrayInstrPush(arr, directions);
}

int main(void) {
  AuxArrayInstr instructions;
  AuxArrayInstrCreate(&instructions, 8);
  AuxReadFileLineByLine("day02/input.txt", parse_line, &instructions);

  for (size_t i = 0; i < instructions.length; ++i) {
    AuxArrayDirectionDestroy(&instructions.items[i]);
  }
  AuxArrayInstrDestroy(&instructions);
}
