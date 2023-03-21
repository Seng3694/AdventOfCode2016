#include <aux.h>
#include <string.h>
#include <ctype.h>

#define MATERIAL_TYPES 8 // only 5 in inputs
#define FLOOR_COUNT 4

static const char *G_COLORS[] = {
    "\e[0;31m", "\e[0;32m", "\e[0;33m", "\e[0;34m",
    "\e[0;35m", "\e[0;36m", "\e[0;37m",
};

static const char *MC_COLORS[] = {
    "\e[1;31m", "\e[1;32m", "\e[1;33m", "\e[1;34m",
    "\e[1;35m", "\e[1;36m", "\e[1;37m",
};

typedef struct {
  uint8_t generators;
  uint8_t chips;
} floor;

typedef struct {
  floor floors[FLOOR_COUNT];
  uint8_t elevator;
} facility;

typedef struct {
  char materials[MATERIAL_TYPES][16];
  uint8_t materialCount;
} material_data;

typedef struct {
  facility *const fac;
  material_data *const md;
  uint8_t currentFloor;
} parsing_context;

static uint8_t add_material(const char *const material, const size_t length,
                            parsing_context *const ctx) {
  for (uint8_t i = 0; i < ctx->md->materialCount; ++i) {
    if (memcmp(material, ctx->md->materials[i], length) == 0)
      return i;
  }
  memcpy(ctx->md->materials + ctx->md->materialCount, material, length);
  ctx->md->materialCount++;
  return ctx->md->materialCount - 1;
}

static void parse_line(char *line, size_t length, void *userData) {
  parsing_context *ctx = userData;
  floor *const f = ctx->fac->floors + ctx->currentFloor;

  while (memcmp(line, "contains", 8) != 0)
    line++;

  line += 9; // skip "contains "

  if (*line == 'n') /* nothing relevant */ {
    goto finish;
  }

  while (memcmp(line, "a ", 2) == 0) {
    line += 2; // skip "a "
    const char *const name = line;
    while (isalpha(*line))
      line++;
    const uint8_t matIndex = add_material(name, line - name, ctx);
    if (*line == ' ') /* generator */ {
      line += 10; // skip " generator"
      f->generators = AUX_SET_BIT(f->generators, matIndex);
    } else /* microchip */ {
      line += 21; // skip "-compatible microchip"
      f->chips = AUX_SET_BIT(f->chips, matIndex);
    }
    if (*line == '.') {
      break;
    } else {
      // skip either ", " or " and " or ", and "
      while (memcmp(line, "a ", 2) != 0)
        line++;
    }
  }

finish:
  ctx->currentFloor++;
}

static void print_facility(const facility *const fac,
                           const material_data *const md,
                           const uint8_t elevator) {
  for (uint8_t i = FLOOR_COUNT - 1; i >= 0; --i) {
    if (i == 0xff)
      break;
    printf("\e[1;94m%c\e[0m f%u: ", elevator == i ? '>' : ' ', i + 1);
    for (uint8_t j = 0; j < md->materialCount; ++j) {
      if (AUX_CHECK_BIT(fac->floors[i].generators, j))
        printf("%s%.*sg \e[0m", G_COLORS[j], 2, md->materials[j]);
      else
        printf(" .  ");

      if (AUX_CHECK_BIT(fac->floors[i].chips, j))
        printf("%s%.*sm \e[0m", MC_COLORS[j], 2, md->materials[j]);
      else
        printf(" .  ");
    }
    printf("\n");
  }
}

int main(void) {
  facility fac = {0};
  material_data md = {0};
  parsing_context ctx = {
      .fac = &fac,
      .md = &md,
  };
  AuxReadFileLineByLine("day11/input.txt", parse_line, &ctx);

  print_facility(&fac, &md, 0);
}
