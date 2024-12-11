#ifndef DAY11_H
#define DAY11_H

#include "aoc.h"
#include <sys/types.h>

int solve_day11(const char *input);
LongArray *get_numbers(char *input);
u_long execute(char *input, int times);
u_long number_blink(u_long num, int num_executions);
void init();

#endif
