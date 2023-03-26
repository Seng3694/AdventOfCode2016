#include <aux.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {
  INSTR_RECT,
  INSTR_ROT_ROW,
  INSTR_ROT_COLUMN,
} instruction_type;

typedef struct {
  instruction_type type;
  uint8_t a;
  uint8_t b;
} instruction;

#define AUX_T instruction
#define AUX_T_NAME Instr
#include <aux_array.h>

typedef struct {
  uint8_t width;
  uint8_t height;
  uint8_t *data;
} screen;

static void create_screen(screen *const s, const uint8_t width,
                          const uint8_t height) {
  s->width = width;
  s->height = height;
  s->data = calloc(width * height, sizeof(uint8_t));
}

static void destroy_screen(screen *const s) {
  free(s->data);
}

static inline bool starts_with(const char *const str, const size_t length,
                               const char *const substr,
                               const size_t substrLength) {
  if (length < substrLength)
    return false;
  return memcmp(str, substr, substrLength) == 0;
}

static void parse_line(char *line, size_t length, void *userData) {
  instruction instr = {0};
  if (starts_with(line, length, "rotate c", 8)) {
    instr.type = INSTR_ROT_COLUMN;
    instr.a = strtol(line + 16, &line, 10);
    instr.b = strtol(line + 4, NULL, 10);
  } else if (starts_with(line, length, "rotate r", 8)) {
    instr.type = INSTR_ROT_ROW;
    instr.a = strtol(line + 13, &line, 10);
    instr.b = strtol(line + 4, NULL, 10);
  } else /*rect*/ {
    instr.type = INSTR_RECT;
    instr.a = strtol(line + 5, &line, 10);
    instr.b = strtol(line + 1, NULL, 10);
  }
  AuxArrayInstr *instructions = userData;
  AuxArrayInstrPush(instructions, instr);
}

static void print_screen(const screen *const s) {
  for (uint8_t y = 0; y < s->height; ++y) {
    const size_t rowOffset = y * s->width;
    for (uint8_t x = 0; x < s->width; ++x) {
      uint8_t v = s->data[rowOffset + x];
      printf("%c", v == 0 ? '.' : '#');
    }
    printf("\n");
  }
}

static void rotate_column(screen *const s, const uint8_t column,
                          const uint8_t amount) {
  uint8_t *const colStart = s->data + column;
  for (size_t i = 0; i < amount; ++i) {
    const uint8_t tmp = colStart[(s->height - 1) * s->width];
    for (size_t j = s->height - 1; j > 0; --j) {
      colStart[j * s->width] = colStart[(j - 1) * s->width];
    }
    colStart[0] = tmp;
  }
}

static void rotate_row(screen *const s, const uint8_t row,
                       const uint8_t amount) {
  uint8_t *const rowStart = s->data + (row * s->width);
  for (size_t i = 0; i < amount; ++i) {
    const uint8_t tmp = rowStart[s->width - 1];
    for (size_t j = s->width - 1; j > 0; --j) {
      rowStart[j] = rowStart[j - 1];
    }
    rowStart[0] = tmp;
  }
}

static void enable_rect(screen *const s, const uint8_t w, const uint8_t h) {
  for (size_t y = 0; y < h; ++y) {
    const size_t rowOffset = y * s->width;
    for (size_t x = 0; x < w; ++x) {
      s->data[rowOffset + x] = 1;
    }
  }
}

static uint32_t count_pixels(const screen *const s) {
  uint32_t count = 0;
  const size_t length = s->width * s->height;
  for (size_t i = 0; i < length; ++i)
    count += s->data[i];
  return count;
}

static uint32_t solve_part1(const AuxArrayInstr *const instructions,
                            screen *const s) {
  for (size_t i = 0; i < instructions->length; ++i) {
    const instruction *const instr = &instructions->items[i];
    switch (instr->type) {
    case INSTR_ROT_COLUMN:
      rotate_column(s, instr->a, instr->b);
      break;
    case INSTR_ROT_ROW:
      rotate_row(s, instr->a, instr->b);
      break;
    case INSTR_RECT:
      enable_rect(s, instr->a, instr->b);
      break;
    }
  }
  return count_pixels(s);
}

int main(void) {
  AuxArrayInstr instructions;
  AuxArrayInstrCreate(&instructions, 180);
  AuxReadFileLineByLine("day08/input.txt", parse_line, &instructions);
  screen s = {0};
  create_screen(&s, 50, 6);

  const uint32_t part1 = solve_part1(&instructions, &s);

  printf("%u\n", part1);
  print_screen(&s);

  destroy_screen(&s);
  AuxArrayInstrDestroy(&instructions);
}
