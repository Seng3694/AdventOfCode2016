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

#define INSTR_LIST(code)                                                       \
  code(nop), code(cpy_i_a), code(cpy_i_b), code(cpy_i_c), code(cpy_i_d),       \
      code(cpy_a_a), code(cpy_a_b), code(cpy_a_c), code(cpy_a_d),              \
      code(cpy_b_a), code(cpy_b_b), code(cpy_b_c), code(cpy_b_d),              \
      code(cpy_c_a), code(cpy_c_b), code(cpy_c_c), code(cpy_c_d),              \
      code(cpy_d_a), code(cpy_d_b), code(cpy_d_c), code(cpy_d_d), code(inc_a), \
      code(inc_b), code(inc_c), code(inc_d), code(dec_a), code(dec_b),         \
      code(dec_c), code(dec_d), code(jnz_i_i), code(jnz_a_i), code(jnz_b_i),   \
      code(jnz_c_i), code(jnz_d_i)

#define INSTR_TYPE(v) instr_type_##v
#define INSTR_LABEL(v) instr_##v

typedef enum {
  INSTR_LIST(INSTR_TYPE),
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
      instr.type = INSTR_TYPE(cpy_i_a) + ((*line) - 'a');
    } else {
      instr.type = INSTR_TYPE(cpy_a_a) + 4 * ((*line) - 'a');
      line += 2;
      instr.type += ((*line) - 'a');
    }
    break;
  case 'j': // jnz
    if (custom_isdigit(*line)) {
      instr.type = INSTR_TYPE(jnz_i_i);
      instr.operand1 = strtol(line, &line, 10);
    } else {
      instr.type = INSTR_TYPE(jnz_a_i) + ((*line) - 'a');
    }
    line++;
    instr.operand2 = strtol(line, &line, 10);
    break;
  case 'i': // inc
    instr.type = INSTR_TYPE(inc_a) + ((*line) - 'a');
    instr.operand1 = strtol(line, &line, 10);
    break;
  case 'd': // dec
    instr.type = INSTR_TYPE(dec_a) + ((*line) - 'a');
    instr.operand1 = strtol(line, &line, 10);
    break;
  }

  AuxArrayInstr *instructions = userData;
  AuxArrayInstrPush(instructions, instr);
}

static inline int32_t jnz(const int32_t x, const int32_t y) {
  return x != 0 ? y : 1;
}

static inline void cpy(int32_t *const r, const int32_t value) {
  *r = value;
}

static inline void inc(int32_t *const r) {
  ++(*r);
}

static inline void dec(int32_t *const r) {
  --(*r);
}

static void run(const AuxArrayInstr *const instructions, registers *const r) {
  const instruction *const instr = instructions->items;
  register int32_t pc = -1;

  static void *dispatchTable[] = {
      INSTR_LIST(&&INSTR_LABEL),
  };

#define DISPATCH() goto *dispatchTable[instr[++pc].type]

  DISPATCH();
  for (;;) {
  instr_nop:
    break;
  instr_cpy_i_a:
    cpy(&r->a, instr[pc].operand1);
    DISPATCH();
  instr_cpy_i_b:
    cpy(&r->b, instr[pc].operand1);
    DISPATCH();
  instr_cpy_i_c:
    cpy(&r->c, instr[pc].operand1);
    DISPATCH();
  instr_cpy_i_d:
    cpy(&r->d, instr[pc].operand1);
    DISPATCH();
  instr_cpy_a_a:
    cpy(&r->a, r->a);
    DISPATCH();
  instr_cpy_a_b:
    cpy(&r->b, r->a);
    DISPATCH();
  instr_cpy_a_c:
    cpy(&r->c, r->a);
    DISPATCH();
  instr_cpy_a_d:
    cpy(&r->d, r->a);
    DISPATCH();
  instr_cpy_b_a:
    cpy(&r->a, r->b);
    DISPATCH();
  instr_cpy_b_b:
    cpy(&r->b, r->b);
    DISPATCH();
  instr_cpy_b_c:
    cpy(&r->c, r->b);
    DISPATCH();
  instr_cpy_b_d:
    cpy(&r->d, r->b);
    DISPATCH();
  instr_cpy_c_a:
    cpy(&r->a, r->c);
    DISPATCH();
  instr_cpy_c_b:
    cpy(&r->b, r->c);
    DISPATCH();
  instr_cpy_c_c:
    cpy(&r->c, r->c);
    DISPATCH();
  instr_cpy_c_d:
    cpy(&r->d, r->c);
    DISPATCH();
  instr_cpy_d_a:
    cpy(&r->a, r->d);
    DISPATCH();
  instr_cpy_d_b:
    cpy(&r->b, r->d);
    DISPATCH();
  instr_cpy_d_c:
    cpy(&r->c, r->d);
    DISPATCH();
  instr_cpy_d_d:
    cpy(&r->d, r->d);
    DISPATCH();
  instr_inc_a:
    inc(&r->a);
    DISPATCH();
  instr_inc_b:
    inc(&r->b);
    DISPATCH();
  instr_inc_c:
    inc(&r->c);
    DISPATCH();
  instr_inc_d:
    inc(&r->d);
    DISPATCH();
  instr_dec_a:
    dec(&r->a);
    DISPATCH();
  instr_dec_b:
    dec(&r->b);
    DISPATCH();
  instr_dec_c:
    dec(&r->c);
    DISPATCH();
  instr_dec_d:
    dec(&r->d);
    DISPATCH();
  instr_jnz_i_i:
    pc += jnz(instr[pc].operand1, instr[pc].operand2) - 1;
    DISPATCH();
  instr_jnz_a_i:
    pc += jnz(r->a, instr[pc].operand2) - 1;
    DISPATCH();
  instr_jnz_b_i:
    pc += jnz(r->b, instr[pc].operand2) - 1;
    DISPATCH();
  instr_jnz_c_i:
    pc += jnz(r->c, instr[pc].operand2) - 1;
    DISPATCH();
  instr_jnz_d_i:
    pc += jnz(r->d, instr[pc].operand2) - 1;
    DISPATCH();
  }

#undef DISPATCH
}

int main(void) {
  AuxArrayInstr instructions;
  AuxArrayInstrCreate(&instructions, 32);
  AuxReadFileLineByLine("day12/input.txt", parse_line, &instructions);
  AuxArrayInstrPush(&instructions, (instruction){.type = instr_type_nop});

  registers r = {0};
  run(&instructions, &r);
  const int32_t part1 = r.a;

  r = (registers){.c = 1};
  run(&instructions, &r);
  const int32_t part2 = r.a;

  printf("%d\n", part1);
  printf("%d\n", part2);

  AuxArrayInstrDestroy(&instructions);
}
