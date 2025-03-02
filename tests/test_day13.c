#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include "day13.h"

static void test_get_next_machine(void **state) {
    const char *path = "tests/test_day13_input.txt";
    FILE *file = fopen(path, "r");
    
    machine_t *first = malloc(sizeof(machine_t));
    machine_t *second = malloc(sizeof(machine_t));
    get_next_machine(file, first);
    get_next_machine(file, second);

    assert_float_equal(first->a_x, 94, EPS);
    assert_float_equal(first->a_y, 34, EPS);
    assert_float_equal(first->b_x, 22, EPS);
    assert_float_equal(first->b_y, 67, EPS);
    assert_float_equal(first->t_x, 8400, EPS);
    assert_float_equal(first->t_y, 5400, EPS);

    free(first);

    assert_float_equal(second->a_x, 26, EPS);
    assert_float_equal(second->a_y, 66, EPS);
    assert_float_equal(second->b_x, 67, EPS);
    assert_float_equal(second->b_y, 21, EPS);
    assert_float_equal(second->t_x, 12748, EPS);
    assert_float_equal(second->t_y, 12176, EPS);

    free(second);
    free(file);
}

static void test_solve_first(void **state)
{
    const char *path = "tests/test_day13_input.txt";
    FILE *file = fopen(path, "r");
    
    machine_t *first = malloc(sizeof(machine_t));
    get_next_machine(file, first);

    long val = solve(first);

    assert_int_equal(80 * 3 + 40, val);

    free(file);
    free(first);
}

static void test_sum_moves(void **state)
{
    const char *path = "tests/test_day13_input.txt";
    FILE *file = fopen(path, "r");

    int sum = sum_moves(file, 0);

    assert_int_equal(sum, 480);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_get_next_machine),
        cmocka_unit_test(test_solve_first),
        cmocka_unit_test(test_sum_moves),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);

    return 0;
}
