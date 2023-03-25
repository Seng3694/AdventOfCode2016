#include <aux.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define MATERIAL_TYPES 8 // only 5 in inputs
#define FLOOR_COUNT 4

#ifndef NDEBUG
static const char *G_COLORS[] = {
    "\e[0;31m", "\e[0;32m", "\e[0;33m", "\e[0;34m",
    "\e[0;35m", "\e[0;36m", "\e[0;37m",
};

static const char *MC_COLORS[] = {
    "\e[1;31m", "\e[1;32m", "\e[1;33m", "\e[1;34m",
    "\e[1;35m", "\e[1;36m", "\e[1;37m",
};
#endif

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

static const facility EMPTY_FAC = {0};

static uint32_t calc_facility_hash(const facility *const fac) {
  uint32_t hash = 2166136261u;
  for (size_t i = 0; i < MATERIAL_TYPES; ++i) {
    uint32_t matHash = 58246577;
    for (size_t j = 0; j < FLOOR_COUNT; ++j) {
      if (AUX_CHECK_BIT(fac->floors[j].chips, i)) {
        matHash ^= j * 16777619;
      }
      if (AUX_CHECK_BIT(fac->floors[j].generators, i)) {
        matHash ^= j * 57620617;
      }
    }
    hash += matHash * 13798559;
  }
  hash ^= fac->elevator;
  hash *= 24056203;
  return hash;
}

static inline bool facility_equals(const facility *const a,
                                   const facility *const b) {
  return calc_facility_hash(a) == calc_facility_hash(b);
}

#define AUX_T facility
#define AUX_T_NAME Fac
#define AUX_T_EMPTY EMPTY_FAC
#define AUX_T_HFUNC(x) calc_facility_hash(x)
#define AUX_T_EQUALS(a, b) facility_equals(a, b)
#include <aux_hashset.h>

#define ARRAY_T facility
#define ARRAY_T_NAME Fac
#include <aux_array.h>

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

#ifndef NDEBUG
static void print_facility(const facility *const fac,
                           const material_data *const md) {
  for (uint8_t i = FLOOR_COUNT - 1; i >= 0; --i) {
    if (i == 0xff)
      break;
    printf("\e[1;94m%c\e[0m f%u: ", fac->elevator == i ? '>' : ' ', i + 1);
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
  printf("\n");
}
#define PRINT_FAC(fac, md) print_facility(fac, md)
#else
#define PRINT_FAC(fac, md)
#endif

static bool is_valid_floor(const floor *const f, const uint8_t materialCount) {
  for (uint8_t i = 0; i < materialCount; ++i) {
    if (AUX_CHECK_BIT(f->chips, i) && f->generators != 0 &&
        !AUX_CHECK_BIT(f->generators, i)) {
      return false;
    }
  }
  return true;
}

static void get_valid_actions(const facility *const state, AuxArrayFac *actions,
                              AuxHashsetFac *const actionsHashset,
                              const uint8_t materialCount) {
  AuxArrayFacClear(actions);
  AuxHashsetFacClear(actionsHashset);
  const floor *const f = &state->floors[state->elevator];

  // one generator and/or one chip
  for (int8_t c = -1; c < materialCount; ++c) {
    for (int8_t g = -1; g < materialCount; ++g) {
      if (c == -1 && g == -1)
        continue;
      facility newState = *state;
      const bool hasChip = c != -1 && AUX_CHECK_BIT(f->chips, c);
      const bool hasGenerator = g != -1 && AUX_CHECK_BIT(f->generators, g);

      if (hasChip) {
        newState.floors[state->elevator].chips =
            AUX_CLEAR_BIT(newState.floors[state->elevator].chips, c);
      }

      if (hasGenerator) {
        newState.floors[state->elevator].generators =
            AUX_CLEAR_BIT(newState.floors[state->elevator].generators, g);
      }

      if (!is_valid_floor(&newState.floors[state->elevator], materialCount))
        continue;

      const int8_t currentElevator = (int8_t)state->elevator;
      for (int8_t i = 0; i < FLOOR_COUNT; ++i) {
        if (i == currentElevator || abs(i - currentElevator) > 1)
          continue;

        facility s = newState;
        s.elevator = i;
        if (hasChip) {
          s.floors[i].chips = AUX_SET_BIT(s.floors[i].chips, c);
        }
        if (hasGenerator) {
          s.floors[i].generators = AUX_SET_BIT(s.floors[i].generators, g);
        }

        // check if only the elevator changed. can't move elevator without
        // moving an item
        const uint8_t elevator = s.elevator;
        s.elevator = currentElevator;
        if (facility_equals(state, &s))
          continue;
        else
          s.elevator = elevator;

        uint32_t hash;
        if (is_valid_floor(&s.floors[i], materialCount) &&
            !AuxHashsetFacContains(actionsHashset, s, &hash)) {
          AuxArrayFacPush(actions, s);
          AuxHashsetFacInsertPreHashed(actionsHashset, s, hash);
        }
      }
    }
  }

  // 2 chips
  for (uint8_t c1 = 0; c1 < materialCount; ++c1) {
    for (uint8_t c2 = c1 + 1; c2 < materialCount; ++c2) {
      if (!AUX_CHECK_BIT(f->chips, c1) || !AUX_CHECK_BIT(f->chips, c2))
        continue;

      facility newState = *state;

      newState.floors[state->elevator].chips =
          AUX_CLEAR_BIT(newState.floors[state->elevator].chips, c1);

      newState.floors[state->elevator].chips =
          AUX_CLEAR_BIT(newState.floors[state->elevator].chips, c2);

      if (!is_valid_floor(&newState.floors[state->elevator], materialCount))
        continue;

      const int8_t currentElevator = (int8_t)state->elevator;
      for (int8_t i = 0; i < FLOOR_COUNT; ++i) {
        if (i == currentElevator || abs(i - currentElevator) > 1)
          continue;

        facility s = newState;
        s.elevator = i;
        s.floors[i].chips = AUX_SET_BIT(s.floors[i].chips, c1);
        s.floors[i].chips = AUX_SET_BIT(s.floors[i].chips, c2);

        // check if only the elevator changed. can't move elevator without
        // moving an item
        const uint8_t elevator = s.elevator;
        s.elevator = currentElevator;
        if (facility_equals(state, &s))
          continue;
        else
          s.elevator = elevator;

        uint32_t hash;
        if (is_valid_floor(&s.floors[i], materialCount) &&
            !AuxHashsetFacContains(actionsHashset, s, &hash)) {
          AuxArrayFacPush(actions, s);
          AuxHashsetFacInsertPreHashed(actionsHashset, s, hash);
        }
      }
    }
  }

  // 2 generators
  for (uint8_t g1 = 0; g1 < materialCount; ++g1) {
    for (uint8_t g2 = g1 + 1; g2 < materialCount; ++g2) {
      if (!AUX_CHECK_BIT(f->generators, g1) ||
          !AUX_CHECK_BIT(f->generators, g2))
        continue;

      facility newState = *state;

      newState.floors[state->elevator].generators =
          AUX_CLEAR_BIT(newState.floors[state->elevator].generators, g1);

      newState.floors[state->elevator].generators =
          AUX_CLEAR_BIT(newState.floors[state->elevator].generators, g2);

      if (!is_valid_floor(&newState.floors[state->elevator], materialCount))
        continue;

      const int8_t currentElevator = (int8_t)state->elevator;
      for (int8_t i = 0; i < FLOOR_COUNT; ++i) {
        if (i == currentElevator || abs(i - currentElevator) > 1)
          continue;

        facility s = newState;
        s.elevator = i;
        s.floors[i].generators = AUX_SET_BIT(s.floors[i].generators, g1);
        s.floors[i].generators = AUX_SET_BIT(s.floors[i].generators, g2);

        // check if only the elevator changed. can't move elevator without
        // moving an item
        const uint8_t elevator = s.elevator;
        s.elevator = currentElevator;
        if (facility_equals(state, &s))
          continue;
        else
          s.elevator = elevator;

        uint32_t hash;
        if (is_valid_floor(&s.floors[i], materialCount) &&
            !AuxHashsetFacContains(actionsHashset, s, &hash)) {
          AuxArrayFacPush(actions, s);
          AuxHashsetFacInsertPreHashed(actionsHashset, s, hash);
        }
      }
    }
  }
}

static uint32_t solve(const facility *const startingFacility,
                      const material_data *md) {
  PRINT_FAC(startingFacility, md);
  facility destination = {
      .elevator = 3,
  };
  for (uint8_t i = 0; i < md->materialCount; ++i) {
    destination.floors[3].chips = AUX_SET_BIT(destination.floors[3].chips, i);
    destination.floors[3].generators =
        AUX_SET_BIT(destination.floors[3].generators, i);
  }

  AuxHashsetFac states = {0};
  AuxHashsetFacCreate(&states, 64);
  AuxHashsetFacInsert(&states, *startingFacility);

  AuxArrayFac actions = {0};
  AuxArrayFacCreate(&actions, 256);

  AuxHashsetFac actionsHashset = {0};
  AuxHashsetFacCreate(&actionsHashset, 256);
  get_valid_actions(startingFacility, &actions, &actionsHashset,
                    md->materialCount);

  AuxArrayFac tmpActions = {0};
  AuxArrayFacCreate(&tmpActions, 256);

  uint32_t actionCount = 0;

  while (actions.length > 0) {
    actionCount++;
    const size_t length = actions.length;
    for (size_t i = 0; i < length; ++i) {
      const facility *const current = &actions.items[i];
      if (facility_equals(current, &destination)) {
        goto finish;
      }
      get_valid_actions(current, &tmpActions, &actionsHashset,
                        md->materialCount);
      for (size_t j = 0; j < tmpActions.length; ++j) {
        uint32_t hash;
        const facility *const tmp = &tmpActions.items[j];
        if (!AuxHashsetFacContains(&states, *tmp, &hash)) {
          AuxHashsetFacInsertPreHashed(&states, *tmp, hash);
          AuxArrayFacPush(&actions, *tmp);
        }
      }
    }
    const size_t newLength = actions.length - length;
    for (size_t k = 0; k < newLength; ++k) {
      actions.items[k] = actions.items[length + k];
    }
    actions.length = newLength;
  }

finish:
  AuxArrayFacDestroy(&tmpActions);
  AuxHashsetFacDestroy(&actionsHashset);
  AuxArrayFacDestroy(&actions);
  AuxHashsetFacDestroy(&states);
  return actionCount;
}

int main(void) {
  facility fac = {0};
  material_data md = {0};
  parsing_context ctx = {
      .fac = &fac,
      .md = &md,
  };
  AuxReadFileLineByLine("day11/input.txt", parse_line, &ctx);

  const uint32_t part1 = solve(&fac, &md);

  fac.floors[0].chips = AUX_SET_BIT(fac.floors[0].chips, md.materialCount);
  fac.floors[0].generators =
      AUX_SET_BIT(fac.floors[0].generators, md.materialCount);
  memcpy(&md.materials[md.materialCount++], "elerium", 8);

  fac.floors[0].chips = AUX_SET_BIT(fac.floors[0].chips, md.materialCount);
  fac.floors[0].generators =
      AUX_SET_BIT(fac.floors[0].generators, md.materialCount);
  memcpy(&md.materials[md.materialCount++], "dilithium", 10);

  const uint32_t part2 = solve(&fac, &md);

  printf("%u\n", part1);
  printf("%u\n", part2);
}
