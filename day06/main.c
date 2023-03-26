#include <aux.h>
#include <string.h>

#define CODE_LENGTH 8u

typedef struct {
  char data[CODE_LENGTH];
} code;

#define AUX_T code
#define AUX_T_NAME Code
#include <aux_array.h>

typedef struct {
  int count;
  char code;
} char_info;

static const char_info DEFAULT_CHAR_INFOS[26] = {
    {0, 'a'}, {0, 'b'}, {0, 'c'}, {0, 'd'}, {0, 'e'}, {0, 'f'}, {0, 'g'},
    {0, 'h'}, {0, 'i'}, {0, 'j'}, {0, 'k'}, {0, 'l'}, {0, 'm'}, {0, 'n'},
    {0, 'o'}, {0, 'p'}, {0, 'q'}, {0, 'r'}, {0, 's'}, {0, 't'}, {0, 'u'},
    {0, 'v'}, {0, 'w'}, {0, 'x'}, {0, 'y'}, {0, 'z'},
};

static int char_info_compare(const void *const lhs, const void *const rhs) {
  const char_info *const left = lhs;
  const char_info *const right = rhs;
  const int difference = right->count - left->count;
  return difference == 0 ? left->code - right->code : difference;
}

static void parse_line(char *line, size_t length, void *userData) {
  AuxArrayCode *codes = userData;
  memcpy(codes->items + codes->length, line, CODE_LENGTH);
  codes->length++;
}

static void solve_both_parts(const AuxArrayCode *const codes, char *const part1,
                             char *const part2) {
  char_info infos[CODE_LENGTH][26];
  for (size_t i = 0; i < CODE_LENGTH; ++i)
    memcpy(infos[i], DEFAULT_CHAR_INFOS, sizeof(char_info) * 26);

  for (size_t y = 0; y < codes->length; ++y) {
    for (size_t x = 0; x < CODE_LENGTH; ++x) {
      infos[x][codes->items[y].data[x] - 'a'].count++;
    }
  }

  for (size_t i = 0; i < CODE_LENGTH; ++i)
    qsort(infos[i], 26, sizeof(char_info), char_info_compare);

  for (size_t i = 0; i < CODE_LENGTH; ++i) {
    part1[i] = infos[i][0].code;
    part2[i] = infos[i][25].code;
  }
}

int main(void) {
  AuxArrayCode codes;
  AuxArrayCodeCreate(&codes, 640);
  AuxReadFileLineByLine("day06/input.txt", parse_line, &codes);

  char part1[CODE_LENGTH + 1] = {0};
  char part2[CODE_LENGTH + 1] = {0};

  solve_both_parts(&codes, part1, part2);

  printf("%s\n", part1);
  printf("%s\n", part2);

  AuxArrayCodeDestroy(&codes);
}
