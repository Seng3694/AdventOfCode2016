#include <aoc/aoc.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  int32_t a;
  int32_t b;
  int32_t c;
  int32_t d;
} registers;

#define INSTR_LIST(code)                                                       \
  code(nop), code(cpy_i_a), code(cpy_i_b), code(cpy_i_c), code(cpy_i_d),       \
      code(cpy_a_a), code(cpy_a_b), code(cpy_a_c), code(cpy_a_d),              \
      code(cpy_b_a), code(cpy_b_b), code(cpy_b_c), code(cpy_b_d),              \
      code(cpy_c_a), code(cpy_c_b), code(cpy_c_c), code(cpy_c_d),              \
      code(cpy_d_a), code(cpy_d_b), code(cpy_d_c), code(cpy_d_d), code(inc_a), \
      code(inc_b), code(inc_c), code(inc_d), code(dec_a), code(dec_b),         \
      code(dec_c), code(dec_d), code(jnz_i_i), code(jnz_a_i), code(jnz_b_i),   \
      code(jnz_c_i), code(jnz_d_i), code(jnz_i_a), code(jnz_i_b),              \
      code(jnz_i_c), code(jnz_i_d), code(jnz_a_a), code(jnz_a_b),              \
      code(jnz_a_c), code(jnz_a_d), code(jnz_b_a), code(jnz_b_b),              \
      code(jnz_b_c), code(jnz_b_d), code(jnz_c_a), code(jnz_c_b),              \
      code(jnz_c_c), code(jnz_c_d), code(jnz_d_a), code(jnz_d_b),              \
      code(jnz_d_c), code(jnz_d_d), code(out_i), code(out_a), code(out_b),     \
      code(out_c), code(out_d)

#define INSTR_TYPE(v) instr_type_##v
#define INSTR_LABEL(v) instr_##v

typedef enum {
  INSTR_LIST(INSTR_TYPE),
} instruction_type;

typedef struct {
  int8_t type;
  int16_t operand1;
  int16_t operand2;
} instruction;

#define AOC_T instruction
#define AOC_T_NAME Instr
#include <aoc/array.h>

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
      instr.type = INSTR_TYPE(cpy_i_a) + ((*line) - 'a');
    } else {
      instr.type = INSTR_TYPE(cpy_a_a) + 4 * ((*line) - 'a');
      line += 2;
      instr.type += ((*line) - 'a');
    }
    break;
  case 'j': // jnz
    if (custom_isdigit(*line)) {
      instr.operand1 = strtol(line, &line, 10);
      line++;
      if (custom_isdigit(*line)) {
        instr.type = INSTR_TYPE(jnz_i_i);
        instr.operand2 = strtol(line, &line, 10);
      } else {
        instr.type = INSTR_TYPE(jnz_i_a) + ((*line) - 'a');
      }
    } else {
      const uint8_t reg = (*line) - 'a';
      line += 2;
      if (custom_isdigit(*line)) {
        instr.type = INSTR_TYPE(jnz_a_i) + reg;
        instr.operand2 = strtol(line, &line, 10);
      } else {
        instr.type = INSTR_TYPE(jnz_a_a) + 4 * reg + ((*line) - 'a');
      }
    }
    break;
  case 'i': // inc
    instr.type = INSTR_TYPE(inc_a) + ((*line) - 'a');
    instr.operand1 = strtol(line, &line, 10);
    break;
  case 'd': // dec
    instr.type = INSTR_TYPE(dec_a) + ((*line) - 'a');
    instr.operand1 = strtol(line, &line, 10);
    break;
  case 'o': // out
    if (custom_isdigit(*line)) {
      instr.type = INSTR_TYPE(out_i);
      instr.operand1 = strtol(line, NULL, 10);
    } else {
      instr.type = INSTR_TYPE(out_a) + ((*line) - 'a');
    }
    break;
  }

  AocArrayInstr *instructions = userData;
  AocArrayInstrPush(instructions, instr);
}

static bool run(AocArrayInstr *const instructions, registers *const r) {
  instruction *const instr = instructions->items;
  register int32_t pc = -1;

  int8_t patternBuffer[32] = {0};
  uint8_t bufferCount = 0;

  static void *dispatchTable[] = {
      INSTR_LIST(&&INSTR_LABEL),
  };

#define DISPATCH() goto *dispatchTable[instr[++pc].type]

#define CPY(dst, value)                                                        \
  dst = value;                                                                 \
  DISPATCH()

#define INC(dst)                                                               \
  dst++;                                                                       \
  DISPATCH()

#define DEC(dst)                                                               \
  dst--;                                                                       \
  DISPATCH()

#define JNZ(op1, op2)                                                          \
  pc += op1 != 0 ? op2 - 1 : 0;                                                \
  DISPATCH()

#define OUT(op)                                                                \
  patternBuffer[bufferCount++] = op;                                           \
  if (bufferCount == 32)                                                       \
    break;                                                                     \
  DISPATCH()

  DISPATCH();
  for (;;) {
    INSTR_LABEL(nop) : break;

    INSTR_LABEL(cpy_i_a) : CPY(r->a, instr[pc].operand1);
    INSTR_LABEL(cpy_i_b) : CPY(r->b, instr[pc].operand1);
    INSTR_LABEL(cpy_i_c) : CPY(r->c, instr[pc].operand1);
    INSTR_LABEL(cpy_i_d) : CPY(r->d, instr[pc].operand1);

    INSTR_LABEL(cpy_a_a) : CPY(r->a, r->a);
    INSTR_LABEL(cpy_a_b) : CPY(r->b, r->a);
    INSTR_LABEL(cpy_a_c) : CPY(r->c, r->a);
    INSTR_LABEL(cpy_a_d) : CPY(r->d, r->a);
    INSTR_LABEL(cpy_b_a) : CPY(r->a, r->b);
    INSTR_LABEL(cpy_b_b) : CPY(r->b, r->b);
    INSTR_LABEL(cpy_b_c) : CPY(r->c, r->b);
    INSTR_LABEL(cpy_b_d) : CPY(r->d, r->b);
    INSTR_LABEL(cpy_c_a) : CPY(r->a, r->c);
    INSTR_LABEL(cpy_c_b) : CPY(r->b, r->c);
    INSTR_LABEL(cpy_c_c) : CPY(r->c, r->c);
    INSTR_LABEL(cpy_c_d) : CPY(r->d, r->c);
    INSTR_LABEL(cpy_d_a) : CPY(r->a, r->d);
    INSTR_LABEL(cpy_d_b) : CPY(r->b, r->d);
    INSTR_LABEL(cpy_d_c) : CPY(r->c, r->d);
    INSTR_LABEL(cpy_d_d) : CPY(r->d, r->d);

    INSTR_LABEL(inc_a) : INC(r->a);
    INSTR_LABEL(inc_b) : INC(r->b);
    INSTR_LABEL(inc_c) : INC(r->c);
    INSTR_LABEL(inc_d) : INC(r->d);

    INSTR_LABEL(dec_a) : DEC(r->a);
    INSTR_LABEL(dec_b) : DEC(r->b);
    INSTR_LABEL(dec_c) : DEC(r->c);
    INSTR_LABEL(dec_d) : DEC(r->d);

    INSTR_LABEL(jnz_i_i) : JNZ(instr[pc].operand1, instr[pc].operand2);
    INSTR_LABEL(jnz_a_i) : JNZ(r->a, instr[pc].operand2);
    INSTR_LABEL(jnz_b_i) : JNZ(r->b, instr[pc].operand2);
    INSTR_LABEL(jnz_c_i) : JNZ(r->c, instr[pc].operand2);
    INSTR_LABEL(jnz_d_i) : JNZ(r->d, instr[pc].operand2);

    INSTR_LABEL(jnz_i_a) : JNZ(instr[pc].operand1, r->a);
    INSTR_LABEL(jnz_i_b) : JNZ(instr[pc].operand1, r->b);
    INSTR_LABEL(jnz_i_c) : JNZ(instr[pc].operand1, r->c);
    INSTR_LABEL(jnz_i_d) : JNZ(instr[pc].operand1, r->d);

    INSTR_LABEL(jnz_a_a) : JNZ(r->a, r->a);
    INSTR_LABEL(jnz_a_b) : JNZ(r->a, r->b);
    INSTR_LABEL(jnz_a_c) : JNZ(r->a, r->c);
    INSTR_LABEL(jnz_a_d) : JNZ(r->a, r->d);
    INSTR_LABEL(jnz_b_a) : JNZ(r->b, r->a);
    INSTR_LABEL(jnz_b_b) : JNZ(r->b, r->b);
    INSTR_LABEL(jnz_b_c) : JNZ(r->b, r->c);
    INSTR_LABEL(jnz_b_d) : JNZ(r->b, r->d);
    INSTR_LABEL(jnz_c_a) : JNZ(r->c, r->a);
    INSTR_LABEL(jnz_c_b) : JNZ(r->c, r->b);
    INSTR_LABEL(jnz_c_c) : JNZ(r->c, r->c);
    INSTR_LABEL(jnz_c_d) : JNZ(r->c, r->d);
    INSTR_LABEL(jnz_d_a) : JNZ(r->d, r->a);
    INSTR_LABEL(jnz_d_b) : JNZ(r->d, r->b);
    INSTR_LABEL(jnz_d_c) : JNZ(r->d, r->c);
    INSTR_LABEL(jnz_d_d) : JNZ(r->d, r->d);

    INSTR_LABEL(out_i) : OUT(instr->operand1);
    INSTR_LABEL(out_a) : OUT(r->a);
    INSTR_LABEL(out_b) : OUT(r->b);
    INSTR_LABEL(out_c) : OUT(r->c);
    INSTR_LABEL(out_d) : OUT(r->d);
  }

#undef DISPATCH
#undef CPY
#undef INC
#undef DEC
#undef JNZ
#undef OUT

  for (uint8_t i = 0; i < bufferCount - 1; ++i)
    if (patternBuffer[i] == patternBuffer[i + 1])
      return false;

  return true;
}

int main(void) {
  AocArrayInstr instructions;
  AocArrayInstrCreate(&instructions, 32);
  AocReadFileLineByLine("day25/input.txt", parse_line, &instructions);
  AocArrayInstrPush(&instructions, (instruction){.type = instr_type_nop});

  AocArrayInstr workingCopy;
  AocArrayInstrDuplicate(&workingCopy, &instructions);

  for (int i = 0;; ++i) {
    registers r = {.a = i};
    if (run(&workingCopy, &r)) {
      printf("%d", i);
      break;
    }
    AocArrayInstrCopy(&workingCopy, &instructions);
  }

  AocArrayInstrDestroy(&workingCopy);
  AocArrayInstrDestroy(&instructions);
}
