#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static const position MOVE_POSITIONS[] = {
    [FACE_DIR_NORTH] = { 0, -1},
    [FACE_DIR_EAST]  = { 1,  0},
    [FACE_DIR_SOUTH] = { 0,  1},
    [FACE_DIR_WEST]  = {-1,  0},
};

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
    pos->x += MOVE_POSITIONS[direction].x * amount;
    pos->y += MOVE_POSITIONS[direction].y * amount;
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

static inline uint32_t hash_position(const position pos) {
    uint32_t hash = 2166136261u;
    hash ^= pos.x;
    hash *= 16777619;
    hash ^= pos.y;
    hash *= 16777619;
    return hash;
}

static int solve_part2(const instruction *instructions, const size_t count) {
    position pos = {0, 0};
    face_direction dir = FACE_DIR_NORTH;
    //sum up lengths to know how many points there can be
    size_t positionsCapacity = 0;
    for(size_t i = 0; i < count; ++i) {
        positionsCapacity += instructions[i].length;
    }
    //add 50% extra so probing won't take too long to find empty spots
    positionsCapacity *= 1.5;
    //sentinel value
    const uint32_t unset = 0xdeadb0b1; 
    uint32_t *positions = malloc(sizeof(uint32_t) * positionsCapacity);
    for(size_t i = 0; i < positionsCapacity; ++i) {
        positions[i] = unset;
    }

    uint32_t hash = hash_position(pos);
    size_t index = hash % positionsCapacity;
    positions[index] = hash;

    for(size_t i = 0; i < count; ++i) {
        const instruction *current = &instructions[i];
        const int change = current->direction == TURN_DIR_RIGHT ? 1 : -1; 
        dir = wrap(dir + change, 0, 3);

        for(size_t j = 0; j < current->length; ++j) {
            move(&pos, dir, 1);
            hash = hash_position(pos);
            index = hash % positionsCapacity;
            //probe for free spot
            while(positions[index] != unset && positions[index] != hash) {
                index = (index + 1) % positionsCapacity;
            }
            if (positions[index] == hash) {
                goto done;
            }
            positions[index] = hash;
        }
    }
    
done:
    free(positions);
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
    const int part2 = solve_part2(instructions, instructionCount);

    printf("%d\n", part1);
    printf("%d\n", part2);

    free(instructions);
    free(input);
}
