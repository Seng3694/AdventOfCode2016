#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

typedef enum {
    TURN_DIR_LEFT,
    TURN_DIR_RIGHT,
} turn_direction;

typedef enum {
    FACE_DIR_NORTH,
    FACE_DIR_EAST,
    FACE_DIR_SOUTH,
    FACE_DIR_WEST,
} face_direction;

typedef struct {
    turn_direction direction;
    int length;
} instruction;

typedef struct {
    int x;
    int y;
} position;

static bool read_file_to_string(const char *path, char **output, size_t *length) {
    FILE *file = fopen(path, "r");
    if (!file) {
        return false;
    } 
    fseek(file, 0, SEEK_END);
    const size_t size = ftell(file);
    rewind(file);
    char *content = malloc(size + 1);
    if (!content) {
        fclose(file);
        return false;
    }
    content[size] = '\0';
    fread(content, size, 1, file);
    fclose(file);
    *output = content;
    return true;
}

static void parse(char *input, instruction **output, size_t *outputLength) {
    size_t instructionsCapacity = 128;
    size_t instructionsLength = 0;
    instruction* instructions = malloc(sizeof(instruction) * instructionsCapacity);

    while(*input != '\0') {
        instruction instr = {0};
        instr.direction = *input == 'L' ? TURN_DIR_LEFT : TURN_DIR_RIGHT;
        input++;
        instr.length = (int)strtol(input, &input, 10);
        while(*input != '\0' && (*input == ' ' || *input == ',')) {
            input++;
        }
        if(instructionsLength + 1 > instructionsCapacity) {
            instructionsCapacity *= 2;
            instructions = realloc(instructions, sizeof(instruction) * instructionsCapacity);
        }
        instructions[instructionsLength++] = instr;
    }
    *output = instructions;
    *outputLength = instructionsLength;
}

static inline int wrap(const int value, const int min, const int max) {
    if(value < min) return max;
    else if(value > max) return min;
    else return value;
}

static inline int distance(const position pos) {
    return abs(pos.x) + abs(pos.y);
}

static inline void move(position *pos, const face_direction direction, const int amount) {
    switch(direction) {
    case FACE_DIR_NORTH:
        pos->y -= amount;
        break;
    case FACE_DIR_EAST:
        pos->x += amount;
        break;
    case FACE_DIR_SOUTH:
        pos->y += amount;
        break;
    case FACE_DIR_WEST:
        pos->x -= amount;
        break;
    }
}

static int solve_part1(const instruction *instructions, const size_t count) {
    position pos = {0, 0};
    face_direction dir = FACE_DIR_NORTH;
    for(size_t i = 0; i < count; ++i) {
        const instruction *current = &instructions[i];
        const int change = current->direction == TURN_DIR_RIGHT ? 1 : -1; 
        dir = wrap(dir + change, 0, 3);
        move(&pos, dir, current->length);
    }
    return distance(pos);
}

int main(void) {
    char *input = NULL;
    size_t length = 0;
    if (!read_file_to_string("day01/input.txt", &input, &length)) {
        return EXIT_FAILURE;
    }

    instruction* instructions = NULL;
    size_t instructionCount = 0;
    parse(input, &instructions, &instructionCount);

    const int part1 = solve_part1(instructions, instructionCount);

    printf("%d\n", part1);

    free(instructions);
    free(input);
}
