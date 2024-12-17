#include <math.h>
#include <stdlib.h>
#include "day17.h"

void handle_adv(Calculator *calc, long input) {
    double denominator = pow(2, input);
}

void handle_bxl(Calculator *calc, long input) {
}

void handle_bst(Calculator *calc, long input) {
}

void handle_jnz(Calculator *calc, long input) {
}

void handle_bxc(Calculator *calc, long input) {
}

void handle_out(Calculator *calc, long input) {
}

void handle_bdv(Calculator *calc, long input) {
}

void handle_cdv(Calculator *calc, long input) {
}

void process_calculation(Calculator *calculator, IntArray *program) {
    int i = 0;
    while (i < program->length) {
        int opcode = program->values[i];
        int input = program->values[i + 1];

        switch (opcode) {
            case 0:
                handle_adv(calculator, input);
                break;
            case 1:
                handle_bxl(calculator, input);
                break;
            case 2:
                handle_bst(calculator, input);
                break;
            case 3:
                handle_jnz(calculator, input);
                break;
            case 4:
                handle_bxc(calculator, input);
                break;
            case 5:
                handle_out(calculator, input);
                break;
            case 6:
                handle_bdv(calculator, input);
                break;
            case 7:
                handle_cdv(calculator, input);
                break;
        }

        i += 2;
    }
}

int solve_day17(const char *input) {
    Calculator *calc = malloc(sizeof(Calculator));

    calc->a = 37293246;
    calc->b = 0;
    calc->c = 0;
    calc->instruction_pointer = 0;
    calc->output = init_int_array(10);

    IntArray *calculation = malloc(sizeof(IntArray));
    calculation->length = 16;
    calculation->max_length = 16;
    int p[] = {2, 4, 1, 6, 7, 5, 4, 4, 1, 7, 1, 0, 3, 5, 5, 3, 0};
    calculation->values = p;

    process_calculation(calc, calculation);

    free(calc);
    return 0;
}
