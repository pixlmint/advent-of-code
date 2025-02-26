#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "day17.h"
#include "aoc.h"

long get_value(Calculator *calc, long input) {
    if (input < 4) {
        return input;
    } else {
        switch (input) {
            case 4:
                return calc->a;
            case 5:
                return calc->b;
            case 6:
                return calc->c;
        }
    }
    fprintf(stderr, "Invalid value passed to get_value: %ld\n", input);
    return -1;
}

void process_calculation(Calculator *calculator, IntArray *program, bool break_on_mismatch_output) {
    while (calculator->instruction_pointer < program->length) {
        int opcode = program->values[calculator->instruction_pointer];
        int input = program->values[calculator->instruction_pointer + 1];
        double denominator;

        switch (opcode) {
            case 0:
                denominator = pow(2, get_value(calculator, input));
                calculator->a = (int) (calculator->a / denominator);
                break;
            case 1:
                calculator->b = calculator->b ^ input;
                break;
            case 2:
                calculator->b = get_value(calculator, input) % 8;
                break;
            case 3:
                if (calculator->a == 0) {
                    calculator->instruction_pointer += 2;
                } else {
                    calculator->instruction_pointer = input;
                }
                break;
            case 4:
                calculator->b = calculator->b ^ calculator->c;
                break;
            case 5:
                const float val = get_value(calculator, input) % 8;
                int_array_append(calculator->output, val);
                if (break_on_mismatch_output && (program->length < calculator->output->length || val != program->values[calculator->output->length - 1])) {
                    return;
                }
                break;
            case 6:
                denominator = pow(2, get_value(calculator, input));
                calculator->b = (int) (calculator->a / denominator);
                break;
            case 7:
                denominator = pow(2, get_value(calculator, input));
                calculator->c = (int) (calculator->a / denominator);
                break;
        }

        if (opcode != 3) {
            calculator->instruction_pointer += 2;
        }
    }
}

Calculator *clone_calculator(Calculator *original) {
    Calculator *new = malloc(sizeof(Calculator));
    new->a = original->a;
    new->b = original->b;
    new->c = original->c;
    new->instruction_pointer = original->instruction_pointer;
    new->output = init_int_array(original->output->max_length);

    return new;
}

void free_calculator(Calculator *calc) {
    free_array(calc->output);
    free(calc);
}

int find_registera_value(Calculator *calculator, IntArray *program) {
    Calculator *tmp = clone_calculator(calculator);
    for (uint64_t i = 0; i < UINT64_MAX; i++) {
        tmp->a = i;
        tmp->instruction_pointer = 0;
        free(tmp->output->values);
        tmp->output->values = malloc(sizeof(int) * program->length);
        tmp->output->length = 0;
        if (i % 1000000 == 0) {
            printf("Trying %lu\n", i);
        }
        process_calculation(tmp, program, true);
        if (tmp->output->length == program->length) {
            bool is_valid = true;
            for (int j = 0; j < program->length; j++) {
                if (program->values[j] != tmp->output->values[j]) {
                    is_valid = false;
                    break;
                }
            }
            if (is_valid) {
                free_calculator(tmp);
                return i;
            }
        }
    }

    return -1;
}

Calculator *get_calc() {
    Calculator *calc = malloc(sizeof(Calculator));

    calc->a = 0;
    calc->b = 0;
    calc->c = 0;
    calc->instruction_pointer = 0;
    calc->output = init_int_array(10);

    return calc;
}

/*

| input        | instruction                                  |
| ------------ | -------------------------------------------- |
| 2 (bst) -> 4 | B = A % 8                                    |
| 1 (bxl) -> 6 | B = B XOR 6                                  |
| 7 (cdv) -> 5 | C = A / 2^B                                  |
| 4 (bxc) -> 4 | B = B XOR C                                  |
| 1 (bxl) -> 7 | B = B XOR 7                                  |
| 0 (adv) -> 3 | A = A / 8                                    |
| 5 (out) -> 5 | print B % 8                                  |
| 3 (jnz) -> 0 | A == 0 ? terminate : instruction pointer = 0 |

*/

int solve_day17(const char *input) {
    Calculator *calc = get_calc();
    calc->a = 37293246;

    IntArray *calculation = malloc(sizeof(IntArray));
    calculation->length = 16;
    calculation->max_length = 16;
    int p[] = {2, 4, 1, 6, 7, 5, 4, 4, 1, 7, 0, 3, 5, 5, 3, 0};
    calculation->values = p;

    Perf *perf = perf_init();
    timer_start("total", perf);
    int part1_timer = timer_start("Part 1", perf);
    process_calculation(calc, calculation, false);
    timer_stop(part1_timer, perf);

    printf("Part 1: ");
    for (int i = 0; i < calc->output->length; i++) {
        if (i != 0) {
            printf(",");
        }
        printf("%d", calc->output->values[i]);
    }
    printf("\n");
    free_calculator(calc);
    calc = get_calc();

    timer_start("Part 2", perf);
    int better_a = find_registera_value(calc, calculation);
    printf("Part 2: %d\n", better_a);

    perf_report(perf);
    perf_close(perf);

    free_calculator(calc);
    free(calculation);
    return 0;
}
