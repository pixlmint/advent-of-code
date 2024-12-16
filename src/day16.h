#ifndef DAY16_H
#define DAY16_H
#include "aoc.h"

void **read_map(FILE *file);
int solve_day16(const char *input);
int find_path(IntMatrix *map, PointArray *previous, Point *current, Point *direction, Point *target, int current_score);

#endif
