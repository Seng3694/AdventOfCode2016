#ifndef AOC_AUX_BITS_H
#define AOC_AUX_BITS_H

#include "aux_common.h"

#define AUX_CLEAR_BIT(value, bit) ((value) & ~(1 << (bit)))
#define AUX_SET_BIT(value, bit) ((value) | (1 << (bit)))
#define AUX_TOGGLE_BIT(value, bit) ((value) ^ (1 << (bit)))
#define AUX_CHECK_BIT(value, bit) (((value) & (1 << (bit))) == (1 << (bit)))

uint32_t AuxPopCnt32(uint32_t i);

#endif
