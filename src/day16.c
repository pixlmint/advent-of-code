#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <threads.h>
#include "day16.h"
#include "aoc.h"

void **read_map(FILE *file) {
    int rows = count_lines(file);
    int cols = count_columns(file);
    IntMatrix *map = init_int_matrix(rows, cols);
    Point *start = malloc(sizeof(Point));
    Point *end = malloc(sizeof(Point));
    void **ret = malloc(sizeof(IntMatrix*) + sizeof(Point*) * 2);
    ret[0] = map;
    ret[1] = start;
    ret[2] = end;

    char c;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            c = fgetc(file);
            if (c == '\n') {
                c = fgetc(file);
            }
            if (c == '.') {
                c = '0';
            } else if (c == 'S') {
                start->x = j;
                start->y = i;
                c = '0';
            } else if (c == 'E') {
                end->x = j;
                end->y = i;
            }
            map->data[i][j] = c;
        }
    }

    start->x = start->x -1;

    return ret;
}

char val(IntMatrix *map, Point *point) {
    if (point->y < 0 || point->y >= map->rows) {
        return -1;
    }
    if (point->x < 0 || point->x >= map->cols) {
        return -1;
    }

    return map->data[(int) point->y][(int) point->x];
}

int find_path(IntMatrix *map, PointArray *previous, Point *current, Point *direction, Point *target, int current_score) {
    Point *next = malloc(sizeof(Point));
    next->x = current->x + direction->x;
    next->y = current->y + direction->y;
    char cur = val(map, next);
    printf("Search at %f, %f, current score: %d\n", next->x, next->y, current_score);

    if (point_array_index_of(previous, next) != -1) {
        printf("Already in prev\n");
        return INT_MAX;
    }

    if (next->x == target->x && next->y == target->y) {
        printf("Found Target\n");
        return current_score;
    }

    if (cur == -1) {
        printf("cur == -1\n");
        return INT_MAX;
    }
    if ('#' + cur < current_score && cur != '#' && cur != '0') {
        printf("We were already here with score %d\n", cur - '#' - 1);
        return INT_MAX;
    }

    if (cur == '#') {
        printf("That's a wall\n");
        return INT_MAX;
    }

    Point *l = malloc(sizeof(Point));
    Point *r = malloc(sizeof(Point));

    if (direction->x == 0) {
        l->x = -1;
        r->x = 1;
        l->y = 0;
        r->y = 0;
    } else {
        l->x = 0;
        r->x = 0;
        l->y = 1;
        r->y = -1;
    }

    map->data[(int) next->y][(int) next->x] = '#' + current_score + 1;

    // printf("\n\n");
    // print_matrix_as_char(map);

    PointArray *prev_r = point_array_clone(previous);
    PointArray *prev_l = point_array_clone(previous);

    int d_l = find_path(map, prev_l, next, l, target, current_score + 1000);
    int d_r = find_path(map, prev_r, next, r, target, current_score + 1000);

    point_array_append(previous, next);
    int d_c = find_path(map, previous, next, direction, target, current_score + 1);

    free(r);
    free(l);
    free(next);
    free_point_array(prev_r);
    free_point_array(prev_l);

    return min(min(d_c, d_l), d_r);
}

int solve_day16(const char *input) {
    FILE *file = fopen(input, "r");
    void **data = read_map(file);
    IntMatrix *map = data[0];
    Point *start = data[1];
    Point *end = data[2];
    free(data);
    fclose(file);

    printf("Start: (%f, %f)\nEnd: (%f, %f)\n", start->x, start->y, end->x, end->y);

    Point *dir = malloc(sizeof(Point));
    dir->x = 1;
    dir->y = 0;
    Point *l = malloc(sizeof(Point));
    Point *r = malloc(sizeof(Point));

    if (dir->x == 0) {
        l->x = -1;
        r->x = 1;
        l->y = 0;
        r->y = 0;
    } else {
        l->x = 0;
        r->x = 0;
        l->y = 1;
        r->y = -1;
    }
    // PointArray *prev = init_point_array(10);
    // PointArray *prev_l = init_point_array(10);
    // PointArray *prev_r = init_point_array(10);

    // int d_c = find_path(map, prev, start, dir, end, 0);
    // int d_l = find_path(map, prev_l, start, l, end, 1000);
    // int d_r = find_path(map, prev_r, start, r, end, 1000);

    // print_matrix_as_char(map);
   // int best_score = min(min(d_c, d_l), d_r);
   // if (best_score == INT_MAX) {
   //     best_score = -1;
   // }
   // printf("Score: %d\n", best_score);
   // 
   // free(dir);
   // free(start);
   // free(end);
   // free_point_array(prev);
   // free_point_array(prev_l);
   // free_point_array(prev_r);

    free_matrix(map);
    return 0;
}

