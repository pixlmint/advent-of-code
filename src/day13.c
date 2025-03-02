#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "day13.h"
#include "aoc.h"


void get_next_machine(FILE *file, machine_t *m)
{
    char *line = malloc(sizeof(char) * 100);

    fgets(line, 100, file);
    sscanf(line, LINE1, &m->a_x, &m->a_y);
    fgets(line, 100, file);
    sscanf(line, LINE2, &m->b_x, &m->b_y);
    fgets(line, 100, file);
    sscanf(line, LINET, &m->t_x, &m->t_y);
    fgets(line, 100, file);

    free(line);
}

machine_t *clone_machine(machine_t *original)
{
    machine_t *m = malloc(sizeof(machine_t));
    m->a_x = original->a_x;
    m->a_y = original->a_y;
    m->b_x = original->b_x;
    m->b_y = original->b_y;
    m->t_x = original->t_x;
    m->t_y = original->t_y;

    return m;
}

void print_machine(machine_t *m)
{
    printf("Button A: X+%i, Y+%i\n", (int) m->a_x, (int) m->a_y);
    printf("Button B: X+%i, Y+%i\n", (int) m->b_x, (int) m->b_y);
    printf("Prize: X=%i, Y=%i\n", (int) m->t_x, (int) m->t_y);
}

void lde(float a, float b, float c, float *a0, float *b0)
{
    if ((int) a % (int) b == 0) {
        *a0 = 0;
        *b0 = c / b;
    } else {
        float a1, b1;
        lde(b, (int) a % (int) b, c, &a1, &b1);
        *a0 = b1;
        *b0 = a1 - (a / b) * b1;
    }
}

void solve(machine_t *m)
{
    float det = (m->a_x * m->b_y) - (m->b_x * m->a_y);
    float det_aug = m->a_x * m->t_y - m->t_x * m->a_y;
    

    printf("det: %f\n", det);

    if (det != 0) {
        // Case 1: Only one possible solution
        m->b_x /= m->a_x;
        m->t_x /= m->a_x;
        m->a_x = 1;

        m->b_y -= m->b_x * m->a_y;
        m->t_y -= m->t_x * m->a_y;
        m->a_y = 0;

        m->t_y /= m->b_y;
        m->b_y = 1;

        m->t_x -= m->t_y * m->b_x;
        m->b_x = 0;
    } else if (det_aug == 0 && (int) m->t_x % gcd(m->a_x, m->b_x) != 0) {
        printf("no valid solutions\n");
        // Case 2: No valid solutions
    } else {
        printf("lde\n");
        print_machine(m);
        // Case 3: Many possible solutions, but only one is optimal

        float A0, B0;
        lde(m->a_x, m->b_x, m->t_x, &A0, &B0);
        
        float k1, k2, k;
        k1 = ceil(-A0 / m->b_x);
        k2 = floor(B0 / m->a_x);

        if (m->a_x / m->b_x > 3) {
            if (k1 > k2) {
                k = k1;
            } else {
                k = k2;
            }
        } else {
            if (k1 < k2) {
                k = k1;
            } else {
                k = k2;
            }
        }

        m->t_x = A0 + k * m->b_x;
        m->t_y = B0 - k * m->a_x;
    }
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

int sum_moves(FILE *file)
{
    int num_machines = count_lines(file) / 4;
    int sum = 0;
    machine_t *m = malloc(sizeof(machine_t));

    for (int i = 0; i < num_machines; i++) {
        get_next_machine(file, m);
        int tokens = solve_bruteforce(m);
        if (tokens > 0) {
            sum += tokens;
        }

        /*if (m->t_x - (int)m->t_x == 0 && m->t_y - (int)m->t_y == 0) {*/
        /*    sum += m->t_x * PRICE_A + m->t_y * PRICE_B;*/
        /*}*/
    }

    free(m);
    return sum;
}

int solve_day13(const char *input)
{
    FILE *file = fopen(input, "r");
    Perf *p = perf_init();
    int t_1 = timer_start("Part 1", p);
    int sum = sum_moves(file);
    timer_stop(t_1, p);
    printf("Part 1: %i\n", sum);
    perf_report(p);
    perf_close(p);
    fclose(file);
    return 0;
}
