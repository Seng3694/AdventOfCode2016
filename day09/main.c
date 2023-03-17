#include <aux.h>
#include <ctype.h>
#include <string.h>

typedef struct {
  char *start;
  size_t length;
} slice;

typedef enum {
  TOKEN_TYPE_TEXT,
  TOKEN_TYPE_MARKER,
} token_type;

typedef struct {
  uint16_t length;
  uint16_t frequency;
} marker_data;

typedef struct {
  slice data;
  marker_data marker;
  token_type type;
} token;

#define ARRAY_T token
#define ARRAY_T_NAME Token
#include <aux_array.h>

static void print_token(const char *const input, const token *const t) {
  printf("type: %s\n", t->type == TOKEN_TYPE_MARKER ? "MARKER" : "TEXT");
  if (t->type == TOKEN_TYPE_MARKER)
    printf("marker: %u x %u\n", t->marker.length, t->marker.frequency);
  printf("slice: '%.*s' index=%zu length=%zu\n\n", (int)t->data.length,
         t->data.start, t->data.start - input, t->data.length);
}

static char *parse_text(char *s, AuxArrayToken *const tokens) {
  token t = {
      .type = TOKEN_TYPE_TEXT,
      .data = {.start = s},
  };
  while (isupper(*s))
    s++;
  t.data.length = s - t.data.start;
  AuxArrayTokenPush(tokens, t);
  return s;
}

static char *parse_marker(char *s, AuxArrayToken *const tokens) {
  token t = {
      .type = TOKEN_TYPE_MARKER,
      .data = {.start = s},
  };
  s++;
  t.marker.length = strtol(s, &s, 10);
  s++; // skip x
  t.marker.frequency = strtol(s, &s, 10);
  s++;
  t.data.length = s - t.data.start;
  AuxArrayTokenPush(tokens, t);
  return s;
}

static void parse(char *s, AuxArrayToken *const tokens) {
  while (*s) {
    switch (*s) {
    case '(': {
      s = parse_marker(s, tokens);
      break;
    }
    default: {
      s = parse_text(s, tokens);
      break;
    }
    }
    while (isspace(*s))
      s++;
  }
}

static int32_t solve_part1(const AuxArrayToken *const tokens) {
  int32_t solution = 0;
  for (size_t i = 0; i < tokens->length; ++i) {
    const token *t = &tokens->items[i];
    if (t->type == TOKEN_TYPE_MARKER) {
      solution += t->marker.frequency * t->marker.length;
      int32_t skip = t->marker.length;
      while (i < tokens->length && skip > 0) {
        i++;
        t = &tokens->items[i];
        skip -= t->data.length;
      }
      solution -= skip;
    } else {
      solution += t->data.length;
    }
  }
  return solution;
}

int main(void) {
  char *input = NULL;
  size_t length;
  if (!AuxReadFileToString("day09/input.txt", &input, &length)) {
    return EXIT_FAILURE;
  }

  AuxArrayToken tokens;
  AuxArrayTokenCreate(&tokens, 640);

  parse(input, &tokens);

  const int32_t part1 = solve_part1(&tokens);

  printf("%d\n", part1);

  AuxArrayTokenDestroy(&tokens);
  free(input);
}
