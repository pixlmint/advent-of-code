#ifndef DAY16_H
#define DAY16_H
#include "aoc.h"

void **read_map(FILE *file);
int solve_day16(const char *input);
PathNode *find_path(IntMatrix *map, IntMatrix *visited, PathNode *previous, Point *direction, Point *target, int current_score, int *lowest_score);
PathNode *search_paths(FILE *file);

#endif
