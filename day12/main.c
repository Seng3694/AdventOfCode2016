#include <aux.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
  int32_t a;
  int32_t b;
  int32_t c;
  int32_t d;
} registers;

typedef enum {
  INSTR_TYPE_NOP,

  INSTR_TYPE_CPY_I_A,
  INSTR_TYPE_CPY_I_B,
  INSTR_TYPE_CPY_I_C,
  INSTR_TYPE_CPY_I_D,

  INSTR_TYPE_CPY_A_A,
  INSTR_TYPE_CPY_A_B,
  INSTR_TYPE_CPY_A_C,
  INSTR_TYPE_CPY_A_D,

  INSTR_TYPE_CPY_B_A,
  INSTR_TYPE_CPY_B_B,
  INSTR_TYPE_CPY_B_C,
  INSTR_TYPE_CPY_B_D,

  INSTR_TYPE_CPY_C_A,
  INSTR_TYPE_CPY_C_B,
  INSTR_TYPE_CPY_C_C,
  INSTR_TYPE_CPY_C_D,

  INSTR_TYPE_CPY_D_A,
  INSTR_TYPE_CPY_D_B,
  INSTR_TYPE_CPY_D_C,
  INSTR_TYPE_CPY_D_D,

  INSTR_TYPE_INC_A,
  INSTR_TYPE_INC_B,
  INSTR_TYPE_INC_C,
  INSTR_TYPE_INC_D,

  INSTR_TYPE_DEC_A,
  INSTR_TYPE_DEC_B,
  INSTR_TYPE_DEC_C,
  INSTR_TYPE_DEC_D,

  INSTR_TYPE_JNZ_I_I,

  INSTR_TYPE_JNZ_A_I,
  INSTR_TYPE_JNZ_B_I,
  INSTR_TYPE_JNZ_C_I,
  INSTR_TYPE_JNZ_D_I,
} instruction_type;

typedef struct {
  int8_t type;
  int8_t operand1;
  int8_t operand2;
} instruction;

#define ARRAY_T instruction
#define ARRAY_T_NAME Instr
#include <aux_array.h>

static inline bool custom_isdigit(const char c) {
  return c == '-' || isdigit(c);
}

static void parse_line(char *line, size_t length, void *userData) {
  const char start = *line;
  line += 4; // skip instr name + space
  instruction instr = {0};

  switch (start) {
  case 'c': // cpy
    if (custom_isdigit(*line)) {
      instr.operand1 = strtol(line, &line, 10);
      line++; // whitespace
      instr.type = INSTR_TYPE_CPY_I_A + ((*line) - 'a');
    } else {
      instr.type = INSTR_TYPE_CPY_A_A + 4 * ((*line) - 'a');
      line += 2;
      instr.type += ((*line) - 'a');
    }
    break;
  case 'j': // jnz
    if (custom_isdigit(*line)) {
      instr.type = INSTR_TYPE_JNZ_I_I;
      instr.operand1 = strtol(line, &line, 10);
    } else {
      instr.type = INSTR_TYPE_JNZ_A_I + ((*line) - 'a');
    }
    line++;
    instr.operand2 = strtol(line, &line, 10);
    break;
  case 'i': // inc
    instr.type = INSTR_TYPE_INC_A + ((*line) - 'a');
    instr.operand1 = strtol(line, &line, 10);
    break;
  case 'd': // dec
    instr.type = INSTR_TYPE_DEC_A + ((*line) - 'a');
    instr.operand1 = strtol(line, &line, 10);
    break;
  }

  AuxArrayInstr *instructions = userData;
  AuxArrayInstrPush(instructions, instr);
}

static inline int32_t jnz(const int32_t x, const int32_t y) {
  return x != 0 ? y : 1;
}

static inline int32_t cpy(int32_t *const r, const int32_t value) {
  *r = value;
  return 1;
}

static inline int32_t inc(int32_t *const r) {
  ++(*r);
  return 1;
}

static inline int32_t dec(int32_t *const r) {
  --(*r);
  return 1;
}

static int32_t solve_part1(const AuxArrayInstr *const instructions) {
  const instruction *const instr = instructions->items;
  int32_t pc = 0;
  registers r = {0};
  const instruction *current = &instr[pc];

  do {
    switch (current->type) {
    case INSTR_TYPE_NOP:
      break;
    case INSTR_TYPE_CPY_I_A:
      pc += cpy(&r.a, current->operand1);
      break;
    case INSTR_TYPE_CPY_I_B:
      pc += cpy(&r.b, current->operand1);
      break;
    case INSTR_TYPE_CPY_I_C:
      pc += cpy(&r.c, current->operand1);
      break;
    case INSTR_TYPE_CPY_I_D:
      pc += cpy(&r.d, current->operand1);
      break;
    case INSTR_TYPE_CPY_A_A:
      pc += cpy(&r.a, r.a);
      break;
    case INSTR_TYPE_CPY_A_B:
      pc += cpy(&r.b, r.a);
      break;
    case INSTR_TYPE_CPY_A_C:
      pc += cpy(&r.c, r.a);
      break;
    case INSTR_TYPE_CPY_A_D:
      pc += cpy(&r.d, r.a);
      break;
    case INSTR_TYPE_CPY_B_A:
      pc += cpy(&r.a, r.b);
      break;
    case INSTR_TYPE_CPY_B_B:
      pc += cpy(&r.b, r.b);
      break;
    case INSTR_TYPE_CPY_B_C:
      pc += cpy(&r.c, r.b);
      break;
    case INSTR_TYPE_CPY_B_D:
      pc += cpy(&r.d, r.b);
      break;
    case INSTR_TYPE_CPY_C_A:
      pc += cpy(&r.a, r.c);
      break;
    case INSTR_TYPE_CPY_C_B:
      pc += cpy(&r.b, r.c);
      break;
    case INSTR_TYPE_CPY_C_C:
      pc += cpy(&r.c, r.c);
      break;
    case INSTR_TYPE_CPY_C_D:
      pc += cpy(&r.d, r.c);
      break;
    case INSTR_TYPE_CPY_D_A:
      pc += cpy(&r.a, r.d);
      break;
    case INSTR_TYPE_CPY_D_B:
      pc += cpy(&r.b, r.d);
      break;
    case INSTR_TYPE_CPY_D_C:
      pc += cpy(&r.c, r.d);
      break;
    case INSTR_TYPE_CPY_D_D:
      pc += cpy(&r.d, r.d);
      break;
    case INSTR_TYPE_INC_A:
      pc += inc(&r.a);
      break;
    case INSTR_TYPE_INC_B:
      pc += inc(&r.b);
      break;
    case INSTR_TYPE_INC_C:
      pc += inc(&r.c);
      break;
    case INSTR_TYPE_INC_D:
      pc += inc(&r.d);
      break;
    case INSTR_TYPE_DEC_A:
      pc += dec(&r.a);
      break;
    case INSTR_TYPE_DEC_B:
      pc += dec(&r.b);
      break;
    case INSTR_TYPE_DEC_C:
      pc += dec(&r.c);
      break;
    case INSTR_TYPE_DEC_D:
      pc += dec(&r.d);
      break;
    case INSTR_TYPE_JNZ_I_I:
      pc += jnz(current->operand1, current->operand2);
      break;
    case INSTR_TYPE_JNZ_A_I:
      pc += jnz(r.a, current->operand2);
      break;
    case INSTR_TYPE_JNZ_B_I:
      pc += jnz(r.b, current->operand2);
      break;
    case INSTR_TYPE_JNZ_C_I:
      pc += jnz(r.c, current->operand2);
      break;
    case INSTR_TYPE_JNZ_D_I:
      pc += jnz(r.d, current->operand2);
      break;
    }
    current = &instr[pc];
  } while (current->type != INSTR_TYPE_NOP);

  return r.a;
}

int main(void) {
  AuxArrayInstr instructions;
  AuxArrayInstrCreate(&instructions, 32);
  AuxReadFileLineByLine("day12/input.txt", parse_line, &instructions);
  AuxArrayInstrPush(&instructions, (instruction){.type = INSTR_TYPE_NOP});

  const int32_t part1 = solve_part1(&instructions);

  printf("%d\n", part1);

  AuxArrayInstrDestroy(&instructions);
}
