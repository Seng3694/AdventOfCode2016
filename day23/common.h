#ifndef DAY23_COMMON_H
#define DAY23_COMMON_H

#include <aux.h>
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
      code(jnz_d_c), code(jnz_d_d), code(tgl_a), code(tgl_b), code(tgl_c),     \
      code(tgl_d)

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

#define AUX_T instruction
#define AUX_T_NAME Instr
#include <aux_array.h>

#endif
