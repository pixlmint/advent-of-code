#ifndef DAY17_H
#define DAY17_H

#include "aoc.h"
#include <stdbool.h>
#include <stdint.h>
typedef struct Calculator {
    uint64_t a;
    uint64_t b;
    uint64_t c;
    int instruction_pointer;
    IntArray *output;
} Calculator;

void process_calculation(Calculator *calculator, IntArray *program, bool break_on_mismatch_output);
int find_registera_value(Calculator *calculator, IntArray *program);
int solve_day17(const char *input);

#endif
