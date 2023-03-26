#include "aux_bits.h"

uint32_t AuxPopCnt32(uint32_t i) {
  i = i - ((i >> 1) & 0x55555555);                // add pairs of bits
  i = (i & 0x33333333) + ((i >> 2) & 0x33333333); // quads
  i = (i + (i >> 4)) & 0x0f0f0f0f;                // groups of 8
  return (i * 0x01010101) >> 24;                  // horizontal sum of bytes
}
