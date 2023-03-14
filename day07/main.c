#include <aux.h>
#include <string.h>

typedef struct {
  const char *start;
  int length;
} slice;

typedef struct {
  char line[144];
  slice strings[5];
  slice brackets[3];
  size_t stringsCount;
  size_t bracketsCount;
} ipv7_address;

#define ARRAY_T ipv7_address
#define ARRAY_T_NAME Ipv7
#include <aux_array.h>

static void parse(char *line, size_t length, void *userData) {
  AuxArrayIpv7 *addresses = userData;
  AuxArrayIpv7Push(addresses, (ipv7_address){0});
  ipv7_address *address = AuxArrayIpv7Last(addresses);
  AuxRemoveTrailingWhitespace(line, &length);
  memcpy(address->line, line, length);
  if (*address->line != '[')
    address->strings[0].start = address->line;

  for (size_t i = 0; i < length; ++i) {
    if (address->line[i] == '[') {
      address->brackets[address->bracketsCount].start = &address->line[i + 1];
      if (i > 0) {
        address->strings[address->stringsCount].length =
            &address->line[i] - address->strings[address->stringsCount].start;
        address->stringsCount++;
      }
    } else if (address->line[i] == ']') {
      address->brackets[address->bracketsCount].length =
          &address->line[i] - address->brackets[address->bracketsCount].start;
      address->bracketsCount++;
      if (i + 1 < length) {
        address->strings[address->stringsCount].start = &address->line[i + 1];
      }
    }
  }
  if (address->line[length - 1] != ']') {
    address->strings[address->stringsCount].length =
        (address->line + length) -
        address->strings[address->stringsCount].start;
    address->stringsCount++;
  }
}

static bool has_abba_pattern(const char *const str, const size_t length) {
  for (size_t i = 0; i + 3 < length; ++i) {
    if (str[i] == str[i + 3] && str[i + 1] == str[i + 2] &&
        str[i] != str[i + 1])
      return true;
  }
  return false;
}

static uint32_t solve_part1(const AuxArrayIpv7 *const addresses) {
  uint32_t count = 0;
  for (size_t i = 0; i < addresses->length; ++i) {
    ipv7_address *a = &addresses->items[i];
    for (size_t j = 0; j < a->bracketsCount; ++j) {
      if (has_abba_pattern(a->brackets[j].start, a->brackets[j].length)) {
        goto next_address;
      }
    }
    for (size_t j = 0; j < a->stringsCount; ++j) {
      if (has_abba_pattern(a->strings[j].start, a->strings[j].length)) {
        count++;
        goto next_address;
      }
    }
  next_address:;
  }
  return count;
}

int main(void) {
  AuxArrayIpv7 addresses;
  AuxArrayIpv7Create(&addresses, 2004);
  AuxReadFileLineByLine("day07/input.txt", parse, &addresses);

  const uint32_t part1 = solve_part1(&addresses);
  printf("%u\n", part1);

  AuxArrayIpv7Destroy(&addresses);
}