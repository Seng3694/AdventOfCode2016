#include <aux.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
  INSTR_TYPE_LOW_HIGH_TO_BOT,
  INSTR_TYPE_LOW_TO_OUTPUT_HIGH_TO_BOT,
  INSTR_TYPE_LOW_HIGH_TO_OUTPUT,
} instruction_type;

typedef struct {
  uint8_t type;
  uint8_t toLow;
  uint8_t toHigh;
} instruction;

typedef struct {
  instruction instr;
  uint8_t id;
  uint8_t low;
  uint8_t high;
} bot;

#define BOT_MAX 256

typedef struct {
  bot initialState[BOT_MAX];
  bot bots[BOT_MAX];
  bot *active[BOT_MAX];
  size_t activeCount;
} context;

static inline bool is_bot_empty(const bot *const b) {
  return b->low == 0 && b->high == 0;
}

static inline bool is_bot_ready(const bot *const b) {
  return b->low != 0 && b->high != 0;
}

static inline bool is_bot_17_61(const bot *const b) {
  return b->low == 17 && b->high == 61;
}

static inline void swap(uint8_t *const a, uint8_t *const b) {
  uint8_t tmp = *a;
  *a = *b;
  *b = tmp;
}

static void give_value_to_bot(bot *const b, const uint8_t value) {
  if (is_bot_empty(b)) {
    b->low = value;
  } else {
    b->high = value;
    if (b->low > b->high) {
      swap(&b->low, &b->high);
    }
  }
}

static void parse_line(char *line, size_t length, void *userData) {
  context *ctx = userData;
  if (length > 30) /* give instruction */ {
    // example: "bot 173 gives low to bot 83 and high to bot 202"
    line += 4; // skip "bot"
    const uint8_t botId = (uint8_t)strtol(line, &line, 10);
    bot *const b = ctx->initialState + botId;
    b->id = botId;
    line += 14; // skip " gives low to "
    if (memcmp(line, "bot", 3) == 0) {
      b->instr.type = INSTR_TYPE_LOW_HIGH_TO_BOT; // there is no high to output
      line += 4;                                  // skip "bot ";
      b->instr.toLow = (uint8_t)strtol(line, &line, 10);
      line += 17; // skip " and high to bot "
      b->instr.toHigh = (uint8_t)strtol(line, NULL, 10);
    } else /* output */ {
      line += 7; // skip "output "
      b->instr.toLow = (uint8_t)strtol(line, &line, 10);
      line += 13; // skip " and high to "
      if (memcmp(line, "bot", 3) == 0) {
        b->instr.type = INSTR_TYPE_LOW_TO_OUTPUT_HIGH_TO_BOT;
        line += 4; // skip "bot "
        b->instr.toHigh = (uint8_t)strtol(line, NULL, 10);
      } else /* output */ {
        b->instr.type = INSTR_TYPE_LOW_HIGH_TO_OUTPUT;
        line += 7; // skip "output "
        b->instr.toHigh = (uint8_t)strtol(line, NULL, 10);
      }
    }
  } else /* input instruction */ {
    // example: "value 2 goes to bot 155"
    line += 6; // skip "value "
    const uint8_t value = (uint8_t)strtol(line, &line, 10);
    line += 13; // skip " goes to bot "
    const uint8_t botId = (uint8_t)strtol(line, NULL, 10);
    give_value_to_bot(ctx->initialState + botId, value);
  }
}

static inline void reset_bots(context *const ctx) {
  memcpy(ctx->bots, ctx->initialState, sizeof(bot) * BOT_MAX);
}

static inline void disable_bot(context *const ctx, const size_t index) {
  ctx->active[index] = ctx->active[--ctx->activeCount];
}

static inline void enable_bot(context *const ctx, bot *b) {
  ctx->active[ctx->activeCount++] = b;
}

static void init_active(context *const ctx) {
  for (size_t i = 0; i < BOT_MAX; ++i) {
    if (is_bot_ready(ctx->bots + i)) {
      enable_bot(ctx, ctx->bots + i);
    }
  }
}

static uint8_t solve_part1(context *const ctx) {
  reset_bots(ctx);
  init_active(ctx);
  uint8_t output[32] = {0};

  while (ctx->activeCount > 0) {
    for (int32_t i = (int32_t)ctx->activeCount - 1; i >= 0; --i) {
      bot *const src = ctx->active[i];
      if (is_bot_17_61(src))
        return src->id;

      switch (src->instr.type) {
      case INSTR_TYPE_LOW_HIGH_TO_BOT: {
        bot *const low = &ctx->bots[src->instr.toLow];
        bot *const high = &ctx->bots[src->instr.toHigh];
        give_value_to_bot(low, src->low);
        give_value_to_bot(high, src->high);
        if (is_bot_ready(low))
          enable_bot(ctx, low);
        if (is_bot_ready(high))
          enable_bot(ctx, high);
      } break;
      case INSTR_TYPE_LOW_HIGH_TO_OUTPUT:
        output[src->instr.toLow] = src->low;
        output[src->instr.toHigh] = src->high;
        break;
      case INSTR_TYPE_LOW_TO_OUTPUT_HIGH_TO_BOT: {
        bot *const high = &ctx->bots[src->instr.toHigh];
        output[src->instr.toLow] = src->low;
        give_value_to_bot(high, src->high);
        if (is_bot_ready(high))
          enable_bot(ctx, high);
      } break;
      }
      src->low = 0;
      src->high = 0;
      disable_bot(ctx, i);
    }
  }

  return 0xff; // should never reach
}

int main(void) {
  context ctx = {0};
  AuxReadFileLineByLine("day10/input.txt", parse_line, &ctx);

  const uint8_t part1 = solve_part1(&ctx);
  printf("%u\n", part1);
}
