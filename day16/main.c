#include <aux.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const char inverse_lookup[] = {'1', '0'};

static size_t dragon_curve(const char *const input, const size_t length,
                           char *output, const size_t bufferSize) {
  if (length > bufferSize) {
    memcpy(output, input, bufferSize);
    return bufferSize;
  }
  size_t current = 0;
  output[current++] = '0';

  for (size_t i = 0; (current + length < bufferSize) && i < length;
       ++i, ++current) {
    output[current] = inverse_lookup[input[length - i - 1] - '0'];
  }
  return current;
}

static void fill_disk(const char *const input, size_t length, char *const disk,
                      const size_t diskLength) {
  if (length > diskLength) {
    memcpy(disk, input, diskLength);
    return;
  } else {
    memcpy(disk, input, length);
  }
  size_t i = length;
  while (i < diskLength) {
    i += dragon_curve(disk, i, disk + i, diskLength);
  }
}

static inline char checksum_helper(const char a, const char b) {
  return a == b ? '1' : '0';
}

static size_t checksum(char *const disk, const size_t length) {
  for (size_t i = 0, j = 0; i < length; i += 2, ++j) {
    disk[j] = checksum_helper(disk[i], disk[i + 1]);
  }
  size_t newLength = length / 2;
  disk[newLength] = '\0';
  return newLength;
}

static void solve(const char *const input, const size_t diskLength) {
  char *disk = calloc(1, diskLength + 1);
  fill_disk(input, strlen(input), disk, diskLength);
  size_t length = diskLength;
  while (length % 2 == 0) {
    length = checksum(disk, length);
  }
  printf("%s\n", disk);
  free(disk);
}

int main(void) {
  const char input[] = "10111011111001111";
  solve(input, 272);
  solve(input, 35651584);
}
