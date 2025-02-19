#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "day18.h"
#include "aoc.h"

/*
------------------------
total: 83743.887000ms
part 1: 48.421000ms
part 2: 83695.397000ms
------------------------
*/

PointArray *read_input(const char *input_coordinates) {
    StringArray *lines = str_split(input_coordinates, '\n');
    PointArray *coords = init_point_array(lines->length);

    for (int i = 0; i < lines->length; i++) {
        if (strlen(lines->values[i]) < 2) {
            continue;
        }
        StringArray *split_coords = str_split(lines->values[i], ',');
        Point *p = malloc(sizeof(Point));
        p->x = atof(split_coords->values[0]);
        p->y = atof(split_coords->values[1]);
        point_array_append(coords, p);
        free(p);
        free_string_array(split_coords);
    }
    free_string_array(lines);

    return coords;
}

void simulate_bytes_falling(IntMatrix *map, PointArray *points, size_t first, size_t steps) {
    for (int i = first; i < steps; i++) {
        const Point *p = points->points[i];
        if (p->x >= map->cols) {
            fprintf(stderr, "Invalid x coordinate %f (maximum: %d)", p->x, map->cols);
        } else if (p->y >= map->rows) {
            fprintf(stderr, "Invalid y coordinate %f (maximum: %d)", p->y, map->rows);
        } else {
            map->data[(int) p->y][(int) p->x] = MAPS_WALL;
        }
    }
}

PathNode *dijkstra(IntMatrix *map) {
    Point end = {map->cols - 1, map->cols - 1};
    Point direction = {1, 0};
    int lowest_score = INT_MAX;
    PathNode ***path_matrix = malloc(map->rows * sizeof( PathNode**));
    for (int i = 0; i < map->rows; i++) {
        path_matrix[i] = malloc(map->cols * sizeof(PathNode*));
        for (int j = 0; j < map->cols; j++) {
            path_matrix[i][j] = init_path_node(j, i, 0, NULL);
        }
    }
    PathNode *start = init_path_node(-1, 0, 0, NULL);
    PathNode *target = path_matrix[map->rows - 1][map->cols - 1];
    PathNode *root = find_path(map, path_matrix, start, &direction, &end, 1, &lowest_score);

    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; j++) {
            if (!(i == map->rows - 1 && j == map->cols -1)) {  // do not free the target
                free(path_matrix[i][j]);
            }
        }
        free(path_matrix[i]);
    }
    free(path_matrix);
    free(start);

    return target;
}

size_t count_min_steps(IntMatrix *map) {
    PathNode *target = dijkstra(map);
    int count = target->value - 1;
    free(target);

    return count;
}

PathNode *find_path(IntMatrix *map, PathNode ***visited, PathNode *previous, Point *direction, Point *target, int current_score, int *lowest_score) {
    int new_y = previous->y + (int) direction->y;
    int new_x = previous->x + (int) direction->x;

    if (new_x >= map->cols || new_y >= map->rows || new_x < 0 || new_y < 0) {
        return NULL;
    }

    char cur = map->data[new_y][new_x];

    if (cur == MAPS_WALL) {
        return NULL;
    }

    PathNode *current = visited[new_y][new_x];

    int previous_visit = current->value;
    if (previous_visit <= current_score && previous_visit != 0) {
        // Were already here with lower score
        return NULL;
    } else if (target->x == current->x && target->y == current->y) {
        // Found Target
        *lowest_score = current_score;
        current->reaches_target = true;
        current->value = current_score;
        current->prev = previous;
        return current;
    } else if (current_score >= *lowest_score) {
        // Already bigger than lowest score
        return NULL;
    /*} else if (find_parent_node(previous, current->x, current->y) != NULL && previous->x != current->x && previous->y != current->y) {*/
    /*    // We were already here in the current path*/
    /*    return current;*/
    }
    current->value = current_score;
    current->prev = previous;

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

    // visited->data[(int) current->y][(int) current->x] = current_score;

    current->o = find_path(map, visited, current, direction, target, current_score + 1, lowest_score);
    current->l = find_path(map, visited, current, l, target, current_score + 1, lowest_score);
    current->r = find_path(map, visited, current, r, target, current_score + 1, lowest_score);

    if ((current->o != NULL && current->o->reaches_target) || (current->r != NULL && current->r->reaches_target) || (current->l != NULL && current->l->reaches_target)) {
        current->reaches_target = true;
    }

    free(r);
    free(l);

    return current;
}

Point *get_first_corrupting_byte(IntMatrix *map, PointArray *bytes, int current_count) {
    PathNode *target;
    for (int i = current_count; i < bytes->length; i++) {
        simulate_bytes_falling(map, bytes, i, i + 1);
        target = dijkstra(map);
        if (!target->value) {
            free(target);
            return bytes->points[i];
        }
    }
    if (target) {
        free(target);
    }

    return NULL;
}

int solve_day18(const char *input) {
    char *input_str = read_file(input);
    PointArray *points = read_input(input_str);
    free(input_str);
    IntMatrix *map = init_int_matrix_value(71, 71, 0);
    Perf *perf = perf_init();
    timer_start("total", perf);
    int part1_timer = timer_start("part 1", perf);
    simulate_bytes_falling(map, points, 0, 1024);
    size_t steps = count_min_steps(map);
    timer_stop(part1_timer, perf);
    printf("Part 1 result: %ld\n", steps);

    timer_start("part 2", perf);
    Point *problematic = get_first_corrupting_byte(map, points, 1024);
    printf("Part 2 result: %d,%d\n", (int) problematic->x, (int) problematic->y);
    perf_report(perf);
    perf_close(perf);
    free_point_array(points);
    free_matrix(map);
    return 0;
}
