#ifndef AOC_AUX_FILESYSTEM_H
#define AOC_AUX_FILESYSTEM_H

#include "aux_common.h"

typedef void (*aux_line_func)(char *line, size_t length, void *userData);

bool AuxReadFileLineByLine(const char *path, aux_line_func func,
                           void *userData);
bool AuxReadFileToString(const char *path, char **output, size_t *length);

#endif
