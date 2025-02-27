#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "day17.h"
#include "aoc.h"

typedef enum {adv, bxl, bst, jnz, bxc, out, bdv, cdv} opcode_t;

reg_t get_value(Calculator *calc, int input) {
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
    fprintf(stderr, "Invalid value passed to get_value: %i\n", input);
    return -1;
}

reg_t reg_pow(int base, int power) {
    if (power == 0) {
        return 1;
    }
    reg_t res = base;
    for (int i = 1; i < power; i++) {
        res *= base;
    }
    // printf("Powering %i^%i = %lld\n", base, power, res);
    return res;
}

void process_calculation(Calculator *calculator, IntArray *program, bool break_on_mismatch_output) {
    reg_t denominator;
    reg_t value;
    opcode_t opcode;
    int input;
    while (calculator->instruction_pointer < program->length) {
        opcode = program->values[calculator->instruction_pointer];
        input = program->values[calculator->instruction_pointer + 1];

        switch (opcode) {
            case adv:
                value = get_value(calculator, input);
                if (value < INT_MAX) {
                    denominator = pow(2, value);
                } else {
                    denominator = reg_pow(2, get_value(calculator, input));
                }
                calculator->a = calculator->a / denominator;
                break;
            case bxl:
                calculator->b = calculator->b ^ input;
                break;
            case bst:
                value = get_value(calculator, input);
                calculator->b = value % 8;
                break;
            case jnz:
                if (calculator->a == 0) {
                    calculator->instruction_pointer += 2;
                } else {
                    calculator->instruction_pointer = input;
                }
                break;
            case bxc:
                calculator->b = calculator->b ^ calculator->c;
                break;
            case out:
                value = get_value(calculator, input);
                int_array_append(calculator->output, value % 8);
                if (break_on_mismatch_output && program->length < calculator->output->length) {
                    return;
                }
                break;
            case bdv:
                value = get_value(calculator, input);
                if (value < INT_MAX) {
                    denominator = pow(2, value);
                } else {
                    denominator = reg_pow(2, value);
                }
                calculator->b = calculator->a / denominator;
                break;
            case cdv:
                value = get_value(calculator, input);
                if (value < INT_MAX) {
                    denominator = pow(2, value);
                } else {
                    denominator = reg_pow(2, value);
                }
                calculator->c = calculator->a / denominator;
                break;
        }

        if (opcode != jnz) {
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

reg_t find_registera_value(Calculator *calculator, IntArray *program) {
    Calculator *tmp = clone_calculator(calculator);
    reg_t a = pow(8, program->length - 1);
    int power = program->length - 2;
    bool found = false;
    IntArray *matched = init_int_array(program->length);
    int_array_append(matched, program->values[program->length - 1]);

    while (found == false) {
        a += pow(8, power);
        tmp->a = a;
        tmp->instruction_pointer = 0;
        tmp->output->length = 0;
        process_calculation(tmp, program, false);

        if (int_array_equal(program, tmp->output)) {
            found = true;
        } else {
            const int start_point = tmp->output->length - matched->length;
            bool arrays_equal = true;
            for (int i = 0; i < tmp->output->length - start_point; i++) {
                if (matched->values[i] != tmp->output->values[i + start_point]) {
                    arrays_equal = false;
                    break;
                }
            }
            if (arrays_equal) {
                power -= 1;
                if (power < 0) {
                    power = 0;
                }
                int new_length = matched->length + 1;
                int_array_slice_to(program, matched, program->length - new_length, program->length);
            }
        }
    }

    free_calculator(tmp);
    free_array(matched);
    // free_array(tmp_sliced);

    return a;
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


// | input        | instruction                                  |
// | ------------ | -------------------------------------------- |
// | 2 (bst) -> 4 | B = A % 8                                    |
// | 1 (bxl) -> 6 | B = B XOR 6                                  |
// | 7 (cdv) -> 5 | C = A / 2^B                                  |
// | 4 (bxc) -> 4 | B = B XOR C                                  |
// | 1 (bxl) -> 7 | B = B XOR 7                                  |
// | 0 (adv) -> 3 | A = A / 8                                    |
// | 5 (out) -> 5 | print B % 8                                  |
// | 3 (jnz) -> 0 | A == 0 ? terminate : instruction pointer = 0 |
//
// Final solution copied from here:
// https://github.com/ading2210/advent-of-code-solutions/blob/main/2024/day17/day17.py#L60
//
//
// | total  | 14030.469000ms |
// | Part 1 | 0.005000ms     |
// | Part 2 | 14030.458000ms |

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
    reg_t better_a = find_registera_value(calc, calculation);
    printf("Part 2: %ld\n", better_a);

    perf_report(perf);
    perf_close(perf);

    free_calculator(calc);
    free(calculation);
    return 0;
}
