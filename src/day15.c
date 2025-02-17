#include <stdbool.h>
#include <stddef.h>
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
    return point->x >= 0 && point->y >= 0 && point->x < map->cols && point->y < map->rows;
}

size_t count_boxes(IntMatrix *map) {
    size_t count = 0;
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; j++) {
            const char c = map->data[i][j];
            if (c == 'O') {
                count++;
            } else if (c == '[') {
                count++;
                if (map->data[i][j + 1] != ']') {
                    fprintf(stderr, "Found just half a box\n");
                }
            }
        }
    }

    return count;
}

bool has_wall_in_direction(IntMatrix *map, Point *from, int direction) {
    Point *new_position = get_new_position(from, direction);

    if (!is_in_bounds(map, new_position)) {
        free(new_position);
        fprintf(stderr, "Somehow came out of bounds\n");
        return true;
    }

    char next_c = map->data[(int) new_position->y][(int) new_position->x];
    char curr_c = map->data[(int) from->y][(int) from->x];

    bool has_wall = false;

    if (next_c == '#') {
        has_wall = true;
    } else if (next_c == 'O') {
        has_wall = has_wall_in_direction(map, new_position, direction);
    } else if ((next_c == '[' || next_c == ']')) {
        has_wall = has_wall_in_direction(map, new_position, direction);
        if (direction == MOVE_UP || direction == MOVE_DOWN) {
            if (next_c == '[') {
                if (!has_wall) {
                    Point other_part = {new_position->x + 1, new_position->y};
                    has_wall = has_wall_in_direction(map, &other_part, direction);
                }
            } else {
                if (!has_wall) {
                    Point other_part = {new_position->x - 1, new_position->y};
                    has_wall = has_wall_in_direction(map, &other_part, direction);
                }
            }
        }
    }

    free(new_position);
    return has_wall;
}


bool exec_move(IntMatrix *map, Point *from, int direction) {
    char current_character = map->data[(int) from->y][(int) from->x];

    if (current_character == '.') {
        return true;
    } else if (current_character == '#') {
        return false;
    }

    Point *new_position = get_new_position(from, direction);

    bool successful_move = true;
    if (current_character == 'O' || current_character == '@') {
        successful_move = exec_move(map, new_position, direction);
    } else if (current_character == ']') {
        Point other_part = {from->x - 1, from->y};
        free(new_position);
        return exec_move(map, &other_part, direction);
    } else if (current_character == '[') {
        if (direction != MOVE_RIGHT) {
            successful_move = exec_move(map, new_position, direction);
        } else {
            Point other_point = {from->x + 2, from->y};
            successful_move = exec_move(map, &other_point, direction);
        }
        if (successful_move && direction != MOVE_LEFT) {
            Point other_point = {from->x + 1, from->y};
            Point *next_point = get_new_position(&other_point, direction);
            successful_move = exec_move(map, next_point, direction);
            free(next_point);
        }
    } else {
        fprintf(stderr, "Unknown character: %c\n", current_character);
        free(new_position);
        return false;
    }

    if (successful_move) {
        if (current_character == '[') {
            Point other_point = {from->x + 1, from->y};
            map->data[(int) new_position->y][(int) new_position->x + 1] = ']';
            map->data[(int) other_point.y][(int) other_point.x] = '.';
        }
        map->data[(int) new_position->y][(int) new_position->x] = current_character;
        if (current_character != '[' || direction != MOVE_LEFT) {
            map->data[(int) from->y][(int) from->x] = '.';
        }
    }

    free(new_position);

    return successful_move;
}

bool try_make_move(IntMatrix *map, Point *box, int direction) {
    Point *new_position = get_new_position(box, direction);

    if (!is_in_bounds(map, new_position)) {
        free(new_position);
        return false;
    }

    if (has_wall_in_direction(map, box, direction)) {
        free(new_position);
        return false;
    }

    exec_move(map, box, direction);

    free(new_position);
    return true;
}

void do_moves(IntArray *moves, IntMatrix *map) {
    Point *bot_position = get_bot_position(map);
    if (bot_position == NULL) {
        return;
    }

    for (int i = 0; i < moves->length; i++) {
        if (try_make_move(map, bot_position, moves->values[i])) {
            Point *tmp_bot_position = bot_position;
            bot_position = get_new_position(bot_position, moves->values[i]);
            free(tmp_bot_position);
        }
    }
    free(bot_position);
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

IntMatrix *blowup_map(IntMatrix *input) {
    IntMatrix *new = init_int_matrix(input->rows, input->cols * 2);

    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < input->cols; j++) {
            const char c = input->data[i][j];
            const int x = j * 2;
            if (c == '#') {
                new->data[i][x] = '#';
                new->data[i][x + 1] = '#';
            } else if (c == 'O') {
                new->data[i][x] = '[';
                new->data[i][x + 1] = ']';
            } else if (c == '.') {
                new->data[i][x] = '.';
                new->data[i][x + 1] = '.';
            } else if (c == '@') {
                new->data[i][x] = '@';
                new->data[i][x + 1] = '.';
            }
        }
    }

    return new;
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
            const char c = map->data[i][j];
            if (c == 'O' || c == '[') {
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

    int result = sum_gps(map);
    free_matrix(map);
    free_array(moves);

    return result;
}

int part_2(const char *input) {
    PtrTuple *data = parse_input(input);
    IntMatrix *small_map = data->a;
    IntArray *moves = data->b;
    IntMatrix *map = blowup_map(small_map);
    free_matrix(small_map);
    free(data);

    size_t original_boxes_count = count_boxes(map);
    do_moves(moves, map);
    size_t difference_boxes_count = original_boxes_count - count_boxes(map);
    if (difference_boxes_count != 0) {
        fprintf(stderr, "Unexpected number of boxes, difference: %ld\n", difference_boxes_count);
    }

    int result = sum_gps(map);
    free_matrix(map);
    free_array(moves);

    return result;
}

int solve_day15(const char *input) {
    printf("Part 1: %d\n", part_1(input));
    printf("Part 2: %d\n", part_2(input));
    return 0;
}

