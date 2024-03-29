#include <aoc/aoc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
  INSTR_SWAP_POSITION,
  INSTR_SWAP_CHAR,
  INSTR_ROTATE_LEFT,
  INSTR_ROTATE_RIGHT,
  INSTR_ROTATE_POSITION,
  INSTR_REVERSE_RANGE,
  INSTR_MOVE,
} instruction_type;

typedef struct {
  uint8_t x;
  uint8_t y;
} uint_instr;

typedef struct {
  char a;
  char b;
} char_instr;

typedef struct {
  instruction_type type;
  union {
    uint_instr swpPos;
    char_instr swpChar;
    uint_instr rot;
    char_instr rotPos;
    uint_instr rev;
    uint_instr move;
  };
} instruction;

#define AOC_T instruction
#define AOC_T_NAME Instr
#include <aoc/array.h>

static void swap_chars(char *input, const char a, const char b) {
  do {
    if (*input == a)
      *input = b;
    else if (*input == b)
      *input = a;
  } while (*(input++));
}

static inline void swap_position(char *input, const uint8_t posA,
                                 const uint8_t posB) {
  const char tmp = input[posA];
  input[posA] = input[posB];
  input[posB] = tmp;
}

static void rotate_left(char *input, char *const buffer, const size_t length,
                        uint8_t step) {
  step %= length;
  if (step == 0)
    return;
  memcpy(buffer, input, length);
  const uint8_t rest = length - step;
  memcpy(input, buffer + step, rest);
  memcpy(input + rest, buffer, step);
}

static void rotate_right(char *input, char *const buffer, const size_t length,
                         uint8_t step) {
  step %= length;
  if (step == 0)
    return;
  memcpy(buffer, input, length);
  const uint8_t rest = length - step;
  memcpy(input, buffer + rest, step);
  memcpy(input + step, buffer, rest);
}

static void rotate_based_on_pos(char *input, char *const buffer,
                                const size_t length, const char c) {
  const char *const cp = strchr(input, c);
  if (!cp)
    return;
  const uint8_t index = (uint8_t)(cp - input);
  const uint8_t step = 1 + index + (index >= 4 ? 1 : 0);
  rotate_right(input, buffer, length, step);
}

// example:
//  str length = 8
//  i = index
//  s = step
//  ni = newIndex
//  c = clamped newIndex
//
// i | s | ni | c
// --------------
// 0 | 1 |  1 | 1
// 1 | 2 |  3 | 3
// 2 | 3 |  5 | 5
// 3 | 4 |  7 | 7
// 4 | 6 | 10 | 2
// 5 | 7 | 12 | 4
// 6 | 8 | 14 | 6
// 7 | 9 | 16 | 0
//
// this function translates a "c" to the "s" to undo the rotates
static void undo_rotate_based_on_pos(char *input, char *const buffer,
                                     const size_t length, const char c) {
  const char *const cp = strchr(input, c);
  if (!cp)
    return;
  uint8_t index = (uint8_t)(cp - input);
  uint8_t previousIndex = 0;
  if (index % 2 == 0) {
    if (index == 0)
      index += length;
    index += length;
    previousIndex = (index - 1) / 2;
  } else {
    previousIndex = index / 2;
  }
  const uint8_t step = index - previousIndex;
  rotate_left(input, buffer, length, step);
}

static inline void reverse_range(char *input, const uint8_t from,
                                 const uint8_t to) {
  const uint8_t replaces = (to - from) / 2 + 1;
  for (uint8_t i = 0; i < replaces; ++i)
    swap_position(input, from + i, to - i);
}

static void move_char(char *input, const size_t length, const uint8_t from,
                      const uint8_t to) {
  if (from == to)
    return;
  const char c = input[from];
  // remove
  for (size_t i = from; i < length - 1; ++i) {
    input[i] = input[i + 1];
  }

  // insert
  const size_t rest = length - to;
  for (size_t i = 0; i < rest - 1; ++i) {
    input[length - 1 - i] = input[length - 2 - i];
  }
  input[to] = c;
}

static void parse_line(char *line, size_t length, void *userData) {
  instruction instr = {0};
  if (line[0] == 'r') {
    if (line[1] == 'e') /* reverse */ {
      // "reverse positions 4 through 7"
      line += 18;
      instr.type = INSTR_REVERSE_RANGE;
      instr.rev.x = strtol(line, &line, 10);
      instr.rev.y = strtol(line + 9, NULL, 10);
    } else /* rotate */ {
      line += 7;
      if (line[0] == 'b') /* based on pos */ {
        // "rotate based on position of letter g"
        instr.type = INSTR_ROTATE_POSITION;
        instr.rotPos.a = line[28]; // pos of letter
      } else if (line[0] == 'l') /* left */ {
        // "rotate left 4 steps"
        line += 5;
        instr.type = INSTR_ROTATE_LEFT;
        instr.rot.x = strtol(line, NULL, 10);
      } else /* right */ {
        // "rotate right 4 steps"
        line += 6;
        instr.type = INSTR_ROTATE_RIGHT;
        instr.rot.x = strtol(line, NULL, 10);
      }
    }
  } else if (line[0] == 's') /* swap */ {
    line += 5;
    if (line[0] == 'l') /* letter */ {
      // "swap letter f with letter a"
      instr.type = INSTR_SWAP_CHAR;
      instr.swpChar.a = line[7];
      instr.swpChar.b = line[21];
    } else /* position */ {
      // "swap position 0 with position 1"
      line += 9;
      instr.type = INSTR_SWAP_POSITION;
      instr.swpPos.x = strtol(line, &line, 10);
      instr.swpPos.y = strtol(line + 15, NULL, 10);
    }
  } else if (line[0] == 'm') /* move */ {
    // "move position 6 to position 7"
    line += 14;
    instr.type = INSTR_MOVE;
    instr.move.x = strtol(line, &line, 10);
    instr.move.y = strtol(line + 13, NULL, 10);
  }
  AocArrayInstrPush(userData, instr);
}

void print_instruction(const char *const input,
                       const instruction *const instr) {
  switch (instr->type) {
  case INSTR_SWAP_POSITION:
    printf("swap pos %u %u", instr->swpPos.x, instr->swpPos.y);
    break;
  case INSTR_SWAP_CHAR:
    printf("swap chars %c %c", instr->swpChar.a, instr->swpChar.b);
    break;
  case INSTR_ROTATE_LEFT:
    printf("rot left %u", instr->rot.x);
    break;
  case INSTR_ROTATE_RIGHT:
    printf("rot right %u", instr->rot.x);
    break;
  case INSTR_ROTATE_POSITION:
    const char *const cp = strchr(input, instr->rotPos.a);
    if (!cp)
      return;
    const uint8_t index = (uint8_t)(cp - input);
    const uint8_t amount = 1 + index + (index >= 4 ? 1 : 0);
    printf("rot pos %c (%u) (1+%u+%d=%u)", instr->rotPos.a, index, index,
           (index >= 4 ? 1 : 0), amount);
    break;
  case INSTR_REVERSE_RANGE:
    printf("rev %u %u", instr->rev.x, instr->rev.y);
    break;
  case INSTR_MOVE:
    printf("move %u %u", instr->move.x, instr->move.y);
    break;
  }
}

void scramble(char *const input, const size_t length,
              const AocArrayInstr *const instructions) {
  char *buffer = malloc(length + 1);

  for (size_t i = 0; i < instructions->length; ++i) {
    const instruction *const instr = &instructions->items[i];
    switch (instr->type) {
    case INSTR_SWAP_POSITION:
      swap_position(input, instr->swpPos.x, instr->swpPos.y);
      break;
    case INSTR_SWAP_CHAR:
      swap_chars(input, instr->swpChar.a, instr->swpChar.b);
      break;
    case INSTR_ROTATE_LEFT:
      rotate_left(input, buffer, length, instr->rot.x);
      break;
    case INSTR_ROTATE_RIGHT:
      rotate_right(input, buffer, length, instr->rot.x);
      break;
    case INSTR_ROTATE_POSITION:
      rotate_based_on_pos(input, buffer, length, instr->rotPos.a);
      break;
    case INSTR_REVERSE_RANGE:
      reverse_range(input, instr->rev.x, instr->rev.y);
      break;
    case INSTR_MOVE:
      move_char(input, length, instr->move.x, instr->move.y);
      break;
    }
  }
  free(buffer);
}

void unscramble(char *const input, const size_t length,
                const AocArrayInstr *const instructions) {
  char *buffer = malloc(length + 1);

  for (int64_t i = (int64_t)instructions->length - 1; i >= 0; --i) {
    if (i < 0)
      break;
    const instruction *const instr = &instructions->items[i];
    switch (instr->type) {
    case INSTR_SWAP_POSITION:
      swap_position(input, instr->swpPos.x, instr->swpPos.y);
      break;
    case INSTR_SWAP_CHAR:
      swap_chars(input, instr->swpChar.a, instr->swpChar.b);
      break;
    case INSTR_ROTATE_LEFT:
      rotate_right(input, buffer, length, instr->rot.x);
      break;
    case INSTR_ROTATE_RIGHT:
      rotate_left(input, buffer, length, instr->rot.x);
      break;
    case INSTR_ROTATE_POSITION:
      undo_rotate_based_on_pos(input, buffer, length, instr->rotPos.a);
      break;
    case INSTR_REVERSE_RANGE:
      reverse_range(input, instr->rev.x, instr->rev.y);
      break;
    case INSTR_MOVE:
      move_char(input, length, instr->move.y, instr->move.x);
      break;
    }
  }
  free(buffer);
}

int main(void) {
  AocArrayInstr instructions = {0};
  AocArrayInstrCreate(&instructions, 100);
  AocReadFileLineByLine("day21/input.txt", parse_line, &instructions);

  char input1[] = "abcdefgh";
  const size_t length1 = sizeof(input1) - 1;
  scramble(input1, length1, &instructions);

  char input2[] = "fbgdceah";
  const size_t length2 = sizeof(input2) - 1;
  unscramble(input2, length2, &instructions);

  printf("%s\n", input1);
  printf("%s\n", input2);

  AocArrayInstrDestroy(&instructions);
}
