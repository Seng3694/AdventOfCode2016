#include "aux_filesystem.h"
#include <stdio.h>
#include <stdlib.h>

bool AuxReadFileToString(const char *path, char **output, size_t *length) {
  FILE *file = fopen(path, "r");
  if (!file) {
    AUX_LOG("Failed to open file '%s'\n", path);
    return false;
  }
  fseek(file, 0, SEEK_END);
  const size_t size = ftell(file);
  rewind(file);
  char *content = malloc(size + 1);
  if (!content) {
    AUX_LOG("Failed to allocate %zu bytes\n", size + 1);
    fclose(file);
    return false;
  }
  content[size] = '\0';
  fread(content, size, 1, file);
  fclose(file);
  *output = content;
  return true;
}
