#include "aux_string.h"
#include "aux_common.h"
#include <ctype.h>

void AuxRemoveTrailingWhitespace(char *str, size_t *length) {
  if (*length == 0)
    return;
  char *current = &str[*length - 1];
  while (current >= str && isspace(*current)) {
    *current = '\0';
    current--;
    (*length)--;
  }
}
