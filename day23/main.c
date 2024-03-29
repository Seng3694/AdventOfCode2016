#include "common.h"

#ifndef NDEBUG
#include "debug.h"
#endif

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
  case 't': // tgl
    instr.type = INSTR_TYPE(tgl_a) + ((*line) - 'a');
    break;
  }

  AocArrayInstr *instructions = userData;
  AocArrayInstrPush(instructions, instr);
}

static const uint8_t toggleMap[] = {
    [INSTR_TYPE(nop)] = INSTR_TYPE(nop),
    [INSTR_TYPE(cpy_i_a)] = INSTR_TYPE(jnz_i_a),
    [INSTR_TYPE(cpy_i_b)] = INSTR_TYPE(jnz_i_b),
    [INSTR_TYPE(cpy_i_c)] = INSTR_TYPE(jnz_i_c),
    [INSTR_TYPE(cpy_i_d)] = INSTR_TYPE(jnz_i_d),
    [INSTR_TYPE(cpy_a_a)] = INSTR_TYPE(jnz_a_a),
    [INSTR_TYPE(cpy_a_b)] = INSTR_TYPE(jnz_a_b),
    [INSTR_TYPE(cpy_a_c)] = INSTR_TYPE(jnz_a_c),
    [INSTR_TYPE(cpy_a_d)] = INSTR_TYPE(jnz_a_d),
    [INSTR_TYPE(cpy_b_a)] = INSTR_TYPE(jnz_b_a),
    [INSTR_TYPE(cpy_b_b)] = INSTR_TYPE(jnz_b_b),
    [INSTR_TYPE(cpy_b_c)] = INSTR_TYPE(jnz_b_c),
    [INSTR_TYPE(cpy_b_d)] = INSTR_TYPE(jnz_b_d),
    [INSTR_TYPE(cpy_c_a)] = INSTR_TYPE(jnz_c_a),
    [INSTR_TYPE(cpy_c_b)] = INSTR_TYPE(jnz_c_b),
    [INSTR_TYPE(cpy_c_c)] = INSTR_TYPE(jnz_c_c),
    [INSTR_TYPE(cpy_c_d)] = INSTR_TYPE(jnz_c_d),
    [INSTR_TYPE(cpy_d_a)] = INSTR_TYPE(jnz_d_a),
    [INSTR_TYPE(cpy_d_b)] = INSTR_TYPE(jnz_d_b),
    [INSTR_TYPE(cpy_d_c)] = INSTR_TYPE(jnz_d_c),
    [INSTR_TYPE(cpy_d_d)] = INSTR_TYPE(jnz_d_d),
    [INSTR_TYPE(inc_a)] = INSTR_TYPE(dec_a),
    [INSTR_TYPE(inc_b)] = INSTR_TYPE(dec_b),
    [INSTR_TYPE(inc_c)] = INSTR_TYPE(dec_c),
    [INSTR_TYPE(inc_d)] = INSTR_TYPE(dec_d),
    [INSTR_TYPE(dec_a)] = INSTR_TYPE(inc_a),
    [INSTR_TYPE(dec_b)] = INSTR_TYPE(inc_b),
    [INSTR_TYPE(dec_c)] = INSTR_TYPE(inc_c),
    [INSTR_TYPE(dec_d)] = INSTR_TYPE(inc_d),
    [INSTR_TYPE(jnz_i_i)] = INSTR_TYPE(nop),
    [INSTR_TYPE(jnz_a_i)] = INSTR_TYPE(nop),
    [INSTR_TYPE(jnz_b_i)] = INSTR_TYPE(nop),
    [INSTR_TYPE(jnz_c_i)] = INSTR_TYPE(nop),
    [INSTR_TYPE(jnz_d_i)] = INSTR_TYPE(nop),
    [INSTR_TYPE(jnz_i_a)] = INSTR_TYPE(cpy_i_a),
    [INSTR_TYPE(jnz_i_b)] = INSTR_TYPE(cpy_i_b),
    [INSTR_TYPE(jnz_i_c)] = INSTR_TYPE(cpy_i_c),
    [INSTR_TYPE(jnz_i_d)] = INSTR_TYPE(cpy_i_d),
    [INSTR_TYPE(jnz_a_a)] = INSTR_TYPE(cpy_a_a),
    [INSTR_TYPE(jnz_a_b)] = INSTR_TYPE(cpy_a_b),
    [INSTR_TYPE(jnz_a_c)] = INSTR_TYPE(cpy_a_c),
    [INSTR_TYPE(jnz_a_d)] = INSTR_TYPE(cpy_a_d),
    [INSTR_TYPE(jnz_b_a)] = INSTR_TYPE(cpy_b_a),
    [INSTR_TYPE(jnz_b_b)] = INSTR_TYPE(cpy_b_b),
    [INSTR_TYPE(jnz_b_c)] = INSTR_TYPE(cpy_b_c),
    [INSTR_TYPE(jnz_b_d)] = INSTR_TYPE(cpy_b_d),
    [INSTR_TYPE(jnz_c_a)] = INSTR_TYPE(cpy_c_a),
    [INSTR_TYPE(jnz_c_b)] = INSTR_TYPE(cpy_c_b),
    [INSTR_TYPE(jnz_c_c)] = INSTR_TYPE(cpy_c_c),
    [INSTR_TYPE(jnz_c_d)] = INSTR_TYPE(cpy_c_d),
    [INSTR_TYPE(jnz_d_a)] = INSTR_TYPE(cpy_d_a),
    [INSTR_TYPE(jnz_d_b)] = INSTR_TYPE(cpy_d_b),
    [INSTR_TYPE(jnz_d_c)] = INSTR_TYPE(cpy_d_c),
    [INSTR_TYPE(jnz_d_d)] = INSTR_TYPE(cpy_d_d),
    [INSTR_TYPE(tgl_a)] = INSTR_TYPE(inc_a),
    [INSTR_TYPE(tgl_b)] = INSTR_TYPE(inc_b),
    [INSTR_TYPE(tgl_c)] = INSTR_TYPE(inc_c),
    [INSTR_TYPE(tgl_d)] = INSTR_TYPE(inc_d),
};

static void run(AocArrayInstr *const instructions, registers *const r) {
  instruction *const instr = instructions->items;
  register int32_t pc = -1;

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

#define TGL(op)                                                                \
  {                                                                            \
    const int16_t target = pc + op;                                            \
    if (target >= 0 && target < instructions->length) {                        \
      instr[target].type = toggleMap[instr[target].type];                      \
    }                                                                          \
  }                                                                            \
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

    INSTR_LABEL(tgl_a) : TGL(r->a);
    INSTR_LABEL(tgl_b) : TGL(r->b);
    INSTR_LABEL(tgl_c) : TGL(r->c);
    INSTR_LABEL(tgl_d) : TGL(r->d);
  }

#undef DISPATCH
#undef CPY
#undef INC
#undef DEC
#undef JNZ
#undef TGL
}

int main(void) {
  AocArrayInstr instructions;
  AocArrayInstrCreate(&instructions, 32);
  AocReadFileLineByLine("day23/input.txt", parse_line, &instructions);
  AocArrayInstrPush(&instructions, (instruction){.type = instr_type_nop});

  AocArrayInstr workingCopy;
  AocArrayInstrDuplicate(&workingCopy, &instructions);

  registers r = {.a = 7};
  run(&workingCopy, &r);
  const int32_t part1 = r.a;

#ifndef NDEBUG
  print_program(&workingCopy, &r, workingCopy.length - 1);
#endif

  AocArrayInstrCopy(&workingCopy, &instructions);
  r = (registers){.a = 12};
  run(&workingCopy, &r);
  const int32_t part2 = r.a;

#ifndef NDEBUG
  print_program(&workingCopy, &r, workingCopy.length - 1);
#endif

  printf("%d\n", part1);
  printf("%d\n", part2);

  AocArrayInstrDestroy(&workingCopy);
  AocArrayInstrDestroy(&instructions);
}
