#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "day15.h"
#include "aoc.h"

#define MOVE_UP 1
#define MOVE_RIGHT 2
#define MOVE_DOWN 3
#define MOVE_LEFT 4

int get_move_direction(char move) {
    if (move == '^') {
        return MOVE_UP;
    } else if (move == '>') {
        return MOVE_RIGHT;
    } else if (move == 'v') {
        return MOVE_DOWN;
    } else if (move == '<') {
        return MOVE_LEFT;
    }
    return 0;
}

char get_direction_move(int dir) {
    if (dir == MOVE_UP) {
        return '^';
    } else if (dir == MOVE_LEFT) {
        return '<';
    } else if (dir == MOVE_DOWN) {
        return 'v';
    } else if (dir == MOVE_RIGHT) {
        return '>';
    } else {
        return 'x';
    }
}

IntArray* parse_movelist(char *moves) {
    IntArray *arr = init_int_array(256);
    for (int i = 0; i < strlen(moves); i++) {
        const int dir = get_move_direction(moves[i]);
        if (dir < 5 && dir > 0)  {
            int_array_append(arr, dir);
        }
    }

    return arr;
}

Point *get_bot_position(IntMatrix *map) {
    Point *point = malloc(sizeof(Point));
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; j++) {
            if (map->data[i][j] == '@') {
                point->x = j;
                point->y = i;
                return point;
            }
        }
    }

    fprintf(stderr, "Unable to determine bot position\n");
    free(point);
    return NULL;
}

Point *get_new_position(Point* current_position, int direction) {
    Point *new_position = malloc(sizeof(Point));
    new_position->x = current_position->x;
    new_position->y = current_position->y;
    if (direction == MOVE_UP) {
        new_position->y -= 1;
    } else if (direction == MOVE_LEFT) {
        new_position->x -= 1;
    } else if (direction == MOVE_DOWN) {
        new_position->y += 1;
    } else if (direction == MOVE_RIGHT) {
        new_position->x += 1;
    } else {
        fprintf(stderr, "Unknown direction: %d\n", direction);
    }
    return new_position;
}

bool is_in_bounds(IntMatrix *map, Point *point) {
    return point >= 0 && point >= 0 && point->x < map->rows && point->y < map->cols;
}

bool try_make_move(IntMatrix *map, Point *box, int direction) {
    Point *new_position = get_new_position(box, direction);

    if (!is_in_bounds(map, new_position)) {
        free(new_position);
        return false;
    }

    char next_character = map->data[(int) new_position->y][(int) new_position->x];
    char current_character = map->data[(int) box->y][(int) box->x];

    if (next_character == '.' || (next_character == 'O' && try_make_move(map, new_position, direction))) {
        map->data[(int) new_position->y][(int) new_position->x] = current_character;
        map->data[(int) box->y][(int) box->x] = '.';

        free(new_position);
        return true;
    } else {
        free(new_position);
        return false;
    }
}

void do_moves(IntArray *moves, IntMatrix *map) {
    Point *bot_position = get_bot_position(map);
    if (bot_position == NULL) {
        return;
    }

    /*printf("Initial State:\n");*/
    /*print_matrix_as_char(map);*/

    for (int i = 0; i < moves->length; i++) {
        if (try_make_move(map, bot_position, moves->values[i])) {
            Point *tmp_bot_position = bot_position;
            bot_position = get_new_position(bot_position, moves->values[i]);
            free(tmp_bot_position);
            /*printf("Move %c:\n", get_direction_move(moves->values[i]));*/
            /*print_matrix_as_char(map);*/
            /*printf("---\n");*/
        }
    }
}

typedef struct DynamicString {
    size_t length;
    size_t capacity;
    char *data;
} DynamicString;

DynamicString *create_string(size_t initial_capacity) {
    DynamicString *str = malloc(sizeof(DynamicString));
    str->capacity = initial_capacity;
    str->length = 0;
    str->data = malloc(str->capacity);
    str->data[0] = '\0';
    return str;
}

void append_char(DynamicString *str, char c) {
    if (str->length + 1 >= str->capacity) {
        str->capacity *= 2;
        str->data = realloc(str->data, str->capacity);
    }
    str->data[str->length++] = c;
    str->data[str->length] = '\0';
}

void free_string(DynamicString *str) {
    free(str->data);
    free(str);
}

void *parse_input(const char *file) {
    bool map_read = false;
    char *input = read_file(file);

    IntArray *movesList = init_int_array(20);
    DynamicString *map_str = create_string(16);
    int map_len = 0;
    char last_char = '1';

    for (int i = 0; i < strlen(input); i++) {
        const char cur_char = input[i];
        if (!map_read) {
            if (cur_char == '\n' && last_char == '\n') {
                map_read = true;
            } else {
                append_char(map_str, cur_char);
            }
        } else {
            const int move_value = get_move_direction(cur_char);
            if (move_value < 5 && move_value > 0) {
                int_array_append(movesList, move_value);
            }
        }
        last_char = cur_char;
    }

    IntMatrix *map = str_to_matrix(map_str->data);
    free_string(map_str);
    free(input);

    PtrTuple *ret = init_tuple(map, movesList);
    return ret;
}

int sum_gps(IntMatrix *map) {
    int sum = 0;
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; j++) {
            if (map->data[i][j] == 'O') {
                sum += 100 * i + j; 
            }
        }
    }

    return sum;
}

int part_1(const char *input) {
    PtrTuple *data = parse_input(input);
    IntMatrix *map = data->a;
    IntArray *moves = data->b;
    free(data);

    do_moves(moves, map);
    /*print_matrix_as_char(map);*/

    int result = sum_gps(map);
    free_matrix(map);
    free_array(moves);

    return result;
}

int solve_day15(const char *input) {
    printf("Part 1: %d\n", part_1(input));
    return 0;
}

