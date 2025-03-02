#ifndef DAY13_H
#define DAY13_H

#include <stdbool.h>
#include <sys/types.h>
#include <stdio.h>

typedef long mat_t;
#define LINE1 "Button A: X+%ld, Y+%ld\n"
#define LINE2 "Button B: X+%ld, Y+%ld\n"
#define LINET "Prize: X=%ld, Y=%ld\n\n"
#define EPS 0.001
#define PRICE_A 3
#define PRICE_B 1

typedef struct {
    mat_t a_x;
    mat_t a_y;
    mat_t b_x;
    mat_t b_y;
    mat_t t_x;
    mat_t t_y;
} machine_t;

int solve_day13(const char *input);
void get_next_machine(FILE *file, machine_t *m);
mat_t solve(machine_t *m);
mat_t sum_moves(FILE *file, unsigned long target_offset);

#endif
