#ifndef DAY17_H
#define DAY17_H

#include "aoc.h"
#include <stdbool.h>
#include <stdint.h>

typedef unsigned long reg_t;
typedef struct Calculator {
    reg_t a;
    int b;
    int c;
    int instruction_pointer;
    IntArray *output;
} Calculator;

void process_calculation(Calculator *calculator, IntArray *program, bool break_on_mismatch_output);
reg_t find_registera_value(Calculator *calculator, IntArray *program);
int solve_day17(const char *input);

#endif
