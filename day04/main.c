#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <aux.h>

typedef struct {
  int count;
  char code;
} char_info;

typedef struct {
  char name[60];
  int number;
  char checksum[5];
} room_data;

#define AUX_T room_data
#define AUX_T_NAME Room
#include <aux_array.h>

static const char_info DEFAULT_CHAR_INFOS[26] = {
    {0, 'a'}, {0, 'b'}, {0, 'c'}, {0, 'd'}, {0, 'e'}, {0, 'f'}, {0, 'g'},
    {0, 'h'}, {0, 'i'}, {0, 'j'}, {0, 'k'}, {0, 'l'}, {0, 'm'}, {0, 'n'},
    {0, 'o'}, {0, 'p'}, {0, 'q'}, {0, 'r'}, {0, 's'}, {0, 't'}, {0, 'u'},
    {0, 'v'}, {0, 'w'}, {0, 'x'}, {0, 'y'}, {0, 'z'},
};

static void parse_line(char *line, size_t length, void *userData) {
  room_data data = {0};
  char *start = line;
  while (!isdigit(*line))
    line++;
  memcpy(data.name, start, line - start - 1);
  data.number = strtoul(line, &line, 10);
  memcpy(data.checksum, line + 1, 5);

  AuxArrayRoom *rooms = userData;
  AuxArrayRoomPush(rooms, data);
}

static int char_info_compare(const void *const lhs, const void *const rhs) {
  const char_info *const left = lhs;
  const char_info *const right = rhs;
  const int difference = right->count - left->count;
  return difference == 0 ? left->code - right->code : difference;
}

static inline void count_characters(const char *text, char_info *const counts) {
  char c = 0;
  while ((c = *text++)) {
    if (isalpha(c))
      counts[c - 'a'].count++;
  }
}

static inline bool verify_checksum(const char checksum[const 5],
                                   const char_info chars[const 5]) {
  for (size_t i = 0; i < 5; ++i)
    if (checksum[i] != chars[i].code)
      return false;
  return true;
}

static int solve_part1(const AuxArrayRoom *const rooms) {
  char_info charInfos[26] = {0};
  int sum = 0;
  for (size_t i = 0; i < rooms->length; ++i) {
    memcpy(charInfos, DEFAULT_CHAR_INFOS, sizeof(DEFAULT_CHAR_INFOS));
    const room_data *data = &rooms->items[i];
    count_characters(data->name, charInfos);
    qsort(charInfos, 26, sizeof(char_info), char_info_compare);
    if (verify_checksum(data->checksum, charInfos))
      sum += data->number;
  }
  return sum;
}

static inline char shift_char(const char c, const int amount) {
  return (((c - 'a') + amount) % 26) + 'a';
}

static inline void decrypt(char *text, const size_t length,
                           const size_t shift) {
  for (size_t i = 0; i < length; ++i) {
    if (text[i] == '-')
      text[i] = ' ';
    else
      text[i] = shift_char(text[i], shift);
  }
}

static int solve_part2(AuxArrayRoom *const rooms) {
  const char solution[] = "northpole object storage";
  const size_t solutionLength = sizeof(solution) - 1;
  for (size_t i = 0; i < rooms->length; ++i) {
    room_data *data = &rooms->items[i];
    const size_t length = strlen(data->name);
    if (length != solutionLength)
      continue;
    const int shiftAmount = data->number % 26;
    decrypt(data->name, length, shiftAmount);
    if (memcmp(solution, data->name, length) == 0)
      return data->number;
  }
  return -1;
}

int main(void) {
  AuxArrayRoom rooms;
  AuxArrayRoomCreate(&rooms, 1000);
  AuxReadFileLineByLine("day04/input.txt", parse_line, &rooms);

  const int part1 = solve_part1(&rooms);
  const int part2 = solve_part2(&rooms);

  printf("%d\n", part1);
  printf("%d\n", part2);

  AuxArrayRoomDestroy(&rooms);
}
