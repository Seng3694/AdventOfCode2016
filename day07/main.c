#include <aoc/aoc.h>
#include <string.h>
#include <stdio.h>

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

typedef struct {
  char a;
  char b;
} aba_data;

#define AOC_T ipv7_address
#define AOC_T_NAME Ipv7
#include <aoc/array.h>

#define AOC_T aba_data
#define AOC_T_NAME AbaData
#include <aoc/array.h>

static void parse(char *line, size_t length, void *userData) {
  AocArrayIpv7 *addresses = userData;
  AocArrayIpv7Push(addresses, (ipv7_address){0});
  ipv7_address *address = AocArrayIpv7Last(addresses);
  AocTrimRight(line, &length);
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

static void get_all_aba_patterns(const char *const str, const size_t length,
                                 AocArrayAbaData *const abaDataArr) {
  for (size_t i = 0; i + 2 < length; ++i) {
    if (str[i] == str[i + 2] && str[i] != str[i + 1]) {
      AocArrayAbaDataPush(abaDataArr, (aba_data){.a = str[i], .b = str[i + 1]});
    }
  }
}

static bool has_specific_aba_pattern(const char *const str, const size_t length,
                                     const char a, const char b) {
  for (size_t i = 0; i + 2 < length; ++i) {
    if (str[i] == a && str[i + 1] == b && str[i + 2] == a) {
      return true;
    }
  }
  return false;
}

static uint32_t solve_part1(const AocArrayIpv7 *const addresses) {
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

static uint32_t solve_part2(const AocArrayIpv7 *const addresses) {
  uint32_t count = 0;
  AocArrayAbaData abaDataArr;
  AocArrayAbaDataCreate(&abaDataArr, 16);
  for (size_t i = 0; i < addresses->length; ++i) {
    ipv7_address *adr = &addresses->items[i];
    for (size_t j = 0; j < adr->bracketsCount; ++j) {
      get_all_aba_patterns(adr->brackets[j].start, adr->brackets[j].length,
                           &abaDataArr);
      for (size_t k = 0; k < abaDataArr.length; ++k) {
        const aba_data *const aba = &abaDataArr.items[k];
        for (size_t m = 0; m < adr->stringsCount; ++m) {
          if (has_specific_aba_pattern(adr->strings[m].start,
                                       adr->strings[m].length, aba->b,
                                       aba->a)) {
            count++;
            goto next_address;
          }
        }
      }
    }
  next_address:
    AocArrayAbaDataClear(&abaDataArr);
  }
  AocArrayAbaDataDestroy(&abaDataArr);
  return count;
}

int main(void) {
  AocArrayIpv7 addresses;
  AocArrayIpv7Create(&addresses, 2004);
  AocReadFileLineByLine("day07/input.txt", parse, &addresses);

  const uint32_t part1 = solve_part1(&addresses);
  const uint32_t part2 = solve_part2(&addresses);
  printf("%u\n", part1);
  printf("%u\n", part2);

  AocArrayIpv7Destroy(&addresses);
}