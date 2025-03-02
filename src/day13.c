#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "day13.h"
#include "aoc.h"


void get_next_machine(FILE *file, machine_t *m)
{
    fscanf(file, LINE1, &m->a_x, &m->a_y);
    fscanf(file, LINE2, &m->b_x, &m->b_y);
    fscanf(file, LINET, &m->t_x, &m->t_y);
}

mat_t solve(machine_t *m)
{
    mat_t numerator_x = m->t_x * m->b_y - m->t_y * m->b_x;
    mat_t denonimator_x = m->a_x * m->b_y - m->a_y * m->b_x;

    mat_t numerator_y = m->t_x * m->a_y - m->t_y * m->a_x;
    mat_t denonimator_y = m->a_y * m->b_x - m->a_x * m->b_y;

    if (numerator_x % denonimator_x == 0 && numerator_y % denonimator_y == 0) {
        mat_t a = numerator_x / denonimator_x;
        mat_t b = numerator_y / denonimator_y;

        return PRICE_A * a + PRICE_B * b;
    }

    return -1;
}

int solve_bruteforce(machine_t *m)
{
    int min_tokens = INT_MAX;
    for (int a = 0; a <= 100; a++) {
        for (int b = 0; b <= 100; b++) {
            if (m->a_x * a + m->b_x * b == m->t_x && m->a_y * a + m->b_y * b == m->t_y) {
                min_tokens = min(min_tokens, PRICE_A * a + PRICE_B * b);
            }
        }
    }

    if (min_tokens != INT_MAX) {
        return min_tokens;
    } else {
        return -1;
    }
}

mat_t sum_moves(FILE *file, unsigned long target_offset)
{
    mat_t sum = 0;
    machine_t *m = malloc(sizeof(machine_t));

    while (!feof(file)) {
        get_next_machine(file, m);
        m->t_x += target_offset;
        m->t_y += target_offset;
        mat_t tokens = solve(m);
        if (tokens > 0) {
            sum += tokens;
        }
    }

    free(m);
    return sum;
}

int solve_day13(const char *input)
{
    FILE *file = fopen(input, "r");
    Perf *p = perf_init();
    int t_1 = timer_start("Part 1", p);
    int sum = sum_moves(file, 0);
    timer_stop(t_1, p);
    printf("Part 1: %i\n", sum);
    fclose(file);
    file = fopen(input, "r");
    timer_start("Part 2", p);
    mat_t sum2 = sum_moves(file, 10000000000000);
    printf("Part 2: %ld\n", sum2);
    perf_report(p);
    perf_close(p);
    fclose(file);
    return 0;
}
