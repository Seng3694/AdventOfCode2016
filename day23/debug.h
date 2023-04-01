#ifndef DAY23_DEBUG_H
#define DAY23_DEBUG_H

#include "common.h"

#define C_BRED "\e[1;31m"
#define C_BBLUE "\e[1;34m"
#define C_YELLOW "\e[0;93m"
#define C_GREEN "\e[0;92m"
#define C_RESET "\e[0m"

#define D_IN(instr) C_BBLUE instr C_RESET
#define D_IMM C_YELLOW "%d" C_RESET
#define D_REG(reg) C_GREEN reg C_RESET "(" D_IMM ")"

void print_instruction(const instruction *const instr,
                       const registers *const r) {
  switch (instr->type) {
  case INSTR_TYPE(nop):
    printf(D_IN("nop") "\n");
    break;
  case INSTR_TYPE(cpy_i_a):
    printf(D_IN("cpy") " " D_IMM " " D_REG("a") "\n", instr->operand1, r->a);
    break;
  case INSTR_TYPE(cpy_i_b):
    printf(D_IN("cpy") " " D_IMM " " D_REG("b") "\n", instr->operand1, r->b);
    break;
  case INSTR_TYPE(cpy_i_c):
    printf(D_IN("cpy") " " D_IMM " " D_REG("c") "\n", instr->operand1, r->c);
    break;
  case INSTR_TYPE(cpy_i_d):
    printf(D_IN("cpy") " " D_IMM " " D_REG("d") "\n", instr->operand1, r->d);
    break;
  case INSTR_TYPE(cpy_a_a):
    printf(D_IN("cpy") " " D_REG("a") " " D_REG("a") "\n", r->a, r->a);
    break;
  case INSTR_TYPE(cpy_a_b):
    printf(D_IN("cpy") " " D_REG("a") " " D_REG("b") "\n", r->a, r->b);
    break;
  case INSTR_TYPE(cpy_a_c):
    printf(D_IN("cpy") " " D_REG("a") " " D_REG("c") "\n", r->a, r->c);
    break;
  case INSTR_TYPE(cpy_a_d):
    printf(D_IN("cpy") " " D_REG("a") " " D_REG("d") "\n", r->a, r->d);
    break;
  case INSTR_TYPE(cpy_b_a):
    printf(D_IN("cpy") " " D_REG("b") " " D_REG("a") "\n", r->b, r->a);
    break;
  case INSTR_TYPE(cpy_b_b):
    printf(D_IN("cpy") " " D_REG("b") " " D_REG("b") "\n", r->b, r->b);
    break;
  case INSTR_TYPE(cpy_b_c):
    printf(D_IN("cpy") " " D_REG("b") " " D_REG("c") "\n", r->b, r->c);
    break;
  case INSTR_TYPE(cpy_b_d):
    printf(D_IN("cpy") " " D_REG("b") " " D_REG("d") "\n", r->b, r->d);
    break;
  case INSTR_TYPE(cpy_c_a):
    printf(D_IN("cpy") " " D_REG("c") " " D_REG("a") "\n", r->c, r->a);
    break;
  case INSTR_TYPE(cpy_c_b):
    printf(D_IN("cpy") " " D_REG("c") " " D_REG("b") "\n", r->c, r->b);
    break;
  case INSTR_TYPE(cpy_c_c):
    printf(D_IN("cpy") " " D_REG("c") " " D_REG("c") "\n", r->c, r->c);
    break;
  case INSTR_TYPE(cpy_c_d):
    printf(D_IN("cpy") " " D_REG("c") " " D_REG("d") "\n", r->c, r->d);
    break;
  case INSTR_TYPE(cpy_d_a):
    printf(D_IN("cpy") " " D_REG("d") " " D_REG("a") "\n", r->d, r->a);
    break;
  case INSTR_TYPE(cpy_d_b):
    printf(D_IN("cpy") " " D_REG("d") " " D_REG("b") "\n", r->d, r->b);
    break;
  case INSTR_TYPE(cpy_d_c):
    printf(D_IN("cpy") " " D_REG("d") " " D_REG("c") "\n", r->d, r->c);
    break;
  case INSTR_TYPE(cpy_d_d):
    printf(D_IN("cpy") " " D_REG("d") " " D_REG("d") "\n", r->d, r->a);
    break;
  case INSTR_TYPE(inc_a):
    printf(D_IN("inc") " " D_REG("a") "\n", r->a);
    break;
  case INSTR_TYPE(inc_b):
    printf(D_IN("inc") " " D_REG("b") "\n", r->b);
    break;
  case INSTR_TYPE(inc_c):
    printf(D_IN("inc") " " D_REG("c") "\n", r->c);
    break;
  case INSTR_TYPE(inc_d):
    printf(D_IN("inc") " " D_REG("d") "\n", r->d);
    break;
  case INSTR_TYPE(dec_a):
    printf(D_IN("dec") " " D_REG("a") "\n", r->a);
    break;
  case INSTR_TYPE(dec_b):
    printf(D_IN("dec") " " D_REG("b") "\n", r->b);
    break;
  case INSTR_TYPE(dec_c):
    printf(D_IN("dec") " " D_REG("c") "\n", r->c);
    break;
  case INSTR_TYPE(dec_d):
    printf(D_IN("dec") " " D_REG("d") "\n", r->d);
    break;
  case INSTR_TYPE(jnz_i_i):
    printf(D_IN("jnz") " " D_IMM " " D_IMM "\n", instr->operand1,
           instr->operand2);
    break;
  case INSTR_TYPE(jnz_a_i):
    printf(D_IN("jnz") " " D_REG("a") " " D_IMM "\n", r->a, instr->operand2);
    break;
  case INSTR_TYPE(jnz_b_i):
    printf(D_IN("jnz") " " D_REG("b") " " D_IMM "\n", r->b, instr->operand2);
    break;
  case INSTR_TYPE(jnz_c_i):
    printf(D_IN("jnz") " " D_REG("c") " " D_IMM "\n", r->c, instr->operand2);
    break;
  case INSTR_TYPE(jnz_d_i):
    printf(D_IN("jnz") " " D_REG("d") " " D_IMM "\n", r->d, instr->operand2);
    break;
  case INSTR_TYPE(jnz_i_a):
    printf(D_IN("jnz") " " D_IMM " " D_REG("a") "\n", instr->operand1, r->a);
    break;
  case INSTR_TYPE(jnz_i_b):
    printf(D_IN("jnz") " " D_IMM " " D_REG("b") "\n", instr->operand1, r->b);
    break;
  case INSTR_TYPE(jnz_i_c):
    printf(D_IN("jnz") " " D_IMM " " D_REG("c") "\n", instr->operand1, r->c);
    break;
  case INSTR_TYPE(jnz_i_d):
    printf(D_IN("jnz") " " D_IMM " " D_REG("d") "\n", instr->operand1, r->d);
    break;
  case INSTR_TYPE(jnz_a_a):
    printf(D_IN("jnz") " " D_REG("a") " " D_REG("a") "\n", r->a, r->a);
    break;
  case INSTR_TYPE(jnz_a_b):
    printf(D_IN("jnz") " " D_REG("a") " " D_REG("b") "\n", r->a, r->b);
    break;
  case INSTR_TYPE(jnz_a_c):
    printf(D_IN("jnz") " " D_REG("a") " " D_REG("c") "\n", r->a, r->c);
    break;
  case INSTR_TYPE(jnz_a_d):
    printf(D_IN("jnz") " " D_REG("a") " " D_REG("d") "\n", r->a, r->d);
    break;
  case INSTR_TYPE(jnz_b_a):
    printf(D_IN("jnz") " " D_REG("b") " " D_REG("a") "\n", r->b, r->a);
    break;
  case INSTR_TYPE(jnz_b_b):
    printf(D_IN("jnz") " " D_REG("b") " " D_REG("b") "\n", r->b, r->b);
    break;
  case INSTR_TYPE(jnz_b_c):
    printf(D_IN("jnz") " " D_REG("b") " " D_REG("c") "\n", r->b, r->c);
    break;
  case INSTR_TYPE(jnz_b_d):
    printf(D_IN("jnz") " " D_REG("b") " " D_REG("d") "\n", r->b, r->d);
    break;
  case INSTR_TYPE(jnz_c_a):
    printf(D_IN("jnz") " " D_REG("c") " " D_REG("a") "\n", r->c, r->a);
    break;
  case INSTR_TYPE(jnz_c_b):
    printf(D_IN("jnz") " " D_REG("c") " " D_REG("b") "\n", r->c, r->b);
    break;
  case INSTR_TYPE(jnz_c_c):
    printf(D_IN("jnz") " " D_REG("c") " " D_REG("c") "\n", r->c, r->c);
    break;
  case INSTR_TYPE(jnz_c_d):
    printf(D_IN("jnz") " " D_REG("c") " " D_REG("d") "\n", r->c, r->d);
    break;
  case INSTR_TYPE(jnz_d_a):
    printf(D_IN("jnz") " " D_REG("d") " " D_REG("a") "\n", r->d, r->a);
    break;
  case INSTR_TYPE(jnz_d_b):
    printf(D_IN("jnz") " " D_REG("d") " " D_REG("b") "\n", r->d, r->b);
    break;
  case INSTR_TYPE(jnz_d_c):
    printf(D_IN("jnz") " " D_REG("d") " " D_REG("c") "\n", r->d, r->c);
    break;
  case INSTR_TYPE(jnz_d_d):
    printf(D_IN("jnz") " " D_REG("d") " " D_REG("d") "\n", r->d, r->d);
    break;
  case INSTR_TYPE(tgl_a):
    printf(D_IN("tgl") " " D_REG("a") "\n", r->a);
    break;
  case INSTR_TYPE(tgl_b):
    printf(D_IN("tgl") " " D_REG("b") "\n", r->b);
    break;
  case INSTR_TYPE(tgl_c):
    printf(D_IN("tgl") " " D_REG("c") "\n", r->c);
    break;
  case INSTR_TYPE(tgl_d):
    printf(D_IN("tgl") " " D_REG("d") "\n", r->d);
    break;
  }
}

void print_program(const AuxArrayInstr *const instructions,
                   const registers *const r, const int32_t pc) {
  for (size_t i = 0; i < instructions->length; ++i) {
    const instruction *const instr = &instructions->items[i];

    printf(C_BRED "%s" C_RESET, i == pc ? ">" : " ");
    printf(" %2zu ", i);
    print_instruction(instr, r);
  }
  printf("\n\n");
}

#undef C_BRED
#undef C_BBLUE
#undef C_YELLOW
#undef C_GREEN
#undef C_RESET
#undef D_IN
#undef D_IMM
#undef D_REG

#endif
