#ifndef DAY15_H
#define DAY15_H

#include "aoc.h"

int part_1(const char *input);
int part_2(const char *input);
int solve_day15(const char *input);
size_t count_boxes(IntMatrix *map);
void exec_move(IntMatrix *map, Point *from, int direction);

#endif
