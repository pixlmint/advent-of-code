#ifndef DAY18_H
#define DAY18_H
#include "aoc.h"

typedef struct PathMatrix {
    
} PathMatrix;

int solve_day18(const char *input);
PointArray *read_input(const char *input_coordinates);
PathNode *find_path(IntMatrix *map, PathNode ***visited, PathNode *previous, Point *direction, Point *target, int current_score, int *lowest_score);
size_t count_min_steps(IntMatrix *map);
void simulate_bytes_falling(IntMatrix *map, PointArray *points, size_t first, size_t steps);

#endif
