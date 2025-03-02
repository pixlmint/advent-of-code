#ifndef DAY13_H
#define DAY13_H

#include <stdbool.h>
#include <sys/types.h>
#include <stdio.h>

typedef float mat_t;
#define LINE1 "Button A: X+%f, Y+%f\n"
#define LINE2 "Button B: X+%f, Y+%f\n"
#define LINET "Prize: X=%f, Y=%f\n"
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
void solve(machine_t *m);
int sum_moves(FILE *file);

#endif
