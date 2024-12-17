#ifndef DAY17_H
#define DAY17_H

#include "aoc.h"
typedef struct Calculator {
    long a;
    long b;
    long c;
    int instruction_pointer;
    IntArray *output;
} Calculator;

int solve_day17(const char *input);

#endif
