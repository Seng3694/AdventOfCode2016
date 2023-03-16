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

static char *parse_text(char *s, const size_t minLength,
                        AuxArrayToken *const tokens) {
  token t = {
      .type = TOKEN_TYPE_TEXT,
      .data = {.start = s},
  };
  if (minLength == 0) {
    while (isupper(*s))
      s++;
  } else {
    s += minLength;
  }

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
  return parse_text(s, t.marker.length, tokens);
}

static void parse(char *input, const size_t length,
                  AuxArrayToken *const tokens) {
  char *s = input;
  while (*s) {
    switch (*s) {
    case '(': {
      s = parse_marker(s, tokens);
      break;
    }
    default: {
      s = parse_text(s, 0, tokens);
      break;
    }
    }

    while (isspace(*s))
      s++;
  }
}

static size_t solve_part1(const AuxArrayToken *const tokens) {
  size_t length = 0;
  for (size_t i = 0; i < tokens->length; ++i) {
    const token *const t = &tokens->items[i];
    if (t->type == TOKEN_TYPE_MARKER) {
      length += (t->marker.length * t->marker.frequency);
      i++; // skip next text
    } else {
      length += t->data.length;
    }
  }
  return length;
}

int main(void) {
  char *input = NULL;
  size_t length;
  if (!AuxReadFileToString("day09/input.txt", &input, &length)) {
    return EXIT_FAILURE;
  }

  AuxArrayToken tokens;
  AuxArrayTokenCreate(&tokens, 640);

  parse(input, length, &tokens);

  const size_t part1 = solve_part1(&tokens);

  printf("%zu\n", part1);

  AuxArrayTokenDestroy(&tokens);
  free(input);
}
