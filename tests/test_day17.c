#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include "day17.h"
#include "aoc.h"

Calculator *create_calculator(int a, int b, int c) {
    Calculator *calc = malloc(sizeof(Calculator));

    calc->a = a;
    calc->b = b;
    calc->c = c;
    calc->instruction_pointer = 0;
    calc->output = init_int_array(10);

    return calc;
}

static void test_first_calculation(void **state) {
    Calculator *calc = create_calculator(0, 0, 9);
    IntArray *calculation = init_int_array(2);
    int_array_append(calculation, 2);
    int_array_append(calculation, 6);

    process_calculation(calc, calculation, false);

    assert_int_equal(calc->b, 1);
    
    free_array(calculation);
    free_array(calc->output);
    free(calc);
}

static void test_second_calculation(void **state) {
    Calculator *calc = create_calculator(10, 0, 0);
    IntArray *calculation = init_int_array(6);
    int_array_append(calculation, 5);
    int_array_append(calculation, 0);
    int_array_append(calculation, 5);
    int_array_append(calculation, 1);
    int_array_append(calculation, 5);
    int_array_append(calculation, 4);

    process_calculation(calc, calculation, false);
    
    assert_int_equal(calc->output->length, 3);
    for (int i = 0; i < 3; i++) {
        assert_int_equal(calc->output->values[i], i);
    }
    free_array(calculation);
    free_array(calc->output);
    free(calc);
}

static void test_third_calculation(void **state) {
    Calculator *calc = create_calculator(2024, 0, 0);
    IntArray *calculation = init_int_array(6);
    int_array_append(calculation, 0);
    int_array_append(calculation, 1);
    int_array_append(calculation, 5);
    int_array_append(calculation, 4);
    int_array_append(calculation, 3);
    int_array_append(calculation, 0);

    process_calculation(calc, calculation, false);
    
    assert_int_equal(calc->output->length, 11);
    int exp[] = {4, 2, 5, 6, 7, 7, 7, 7, 3, 1, 0};
    for (int i = 0; i < 11; i++) {
        assert_int_equal(calc->output->values[i], exp[i]);
    }
    assert_int_equal(calc->a, 0);
    free_array(calculation);
    free_array(calc->output);
    free(calc);
}

static void test_fourth_calculation(void **state) {
    Calculator *calc = create_calculator(0, 29, 0);
    IntArray *calculation = init_int_array(2);
    int_array_append(calculation, 1);
    int_array_append(calculation, 7);

    process_calculation(calc, calculation, false);

    assert_int_equal(calc->b, 26);
    
    free_array(calculation);
    free_array(calc->output);
    free(calc);
}

static void test_fifth_calculation(void **state) {
    Calculator *calc = create_calculator(0, 2024, 43690);
    IntArray *calculation = init_int_array(2);
    int_array_append(calculation, 4);
    int_array_append(calculation, 0);

    process_calculation(calc, calculation, false);

    assert_int_equal(calc->b, 44354);
    
    free_array(calculation);
    free_array(calc->output);
    free(calc);
}

static void test_sixth_calculation(void **state) {
    Calculator *calc = create_calculator(729, 0, 0);
    IntArray *calculation = init_int_array(6);
    int_array_append(calculation, 0);
    int_array_append(calculation, 1);
    int_array_append(calculation, 5);
    int_array_append(calculation, 4);
    int_array_append(calculation, 3);
    int_array_append(calculation, 0);

    process_calculation(calc, calculation, false);

    assert_int_equal(calc->output->length, 10);
    int exp[] = {4, 6, 3, 5, 6, 3, 5, 2, 1, 0};
    for (int i = 0; i < 10; i++) {
        assert_int_equal(calc->output->values[i], exp[i]);
    }
    
    free_array(calculation);
    free_array(calc->output);
    free(calc);
}

static void test_seventh_calculation(void **state) {
    Calculator *calc = create_calculator(117440, 0, 0);
    IntArray *calculation = init_int_array(6);
    int_array_append(calculation, 0);
    int_array_append(calculation, 3);
    int_array_append(calculation, 5);
    int_array_append(calculation, 4);
    int_array_append(calculation, 3);
    int_array_append(calculation, 0);

    process_calculation(calc, calculation, false);

    assert_int_equal(calc->output->length, calculation->length);
    for (int i = 0; i < calculation->length; i++) {
        assert_int_equal(calc->output->values[i], calculation->values[i]);
    }
    
    free_array(calculation);
    free_array(calc->output);
    free(calc);
}

static void test_find_register_value(void **state) {
    Calculator *calc = create_calculator(2024, 0, 0);
    IntArray *calculation = init_int_array(6);
    int_array_append(calculation, 0);
    int_array_append(calculation, 3);
    int_array_append(calculation, 5);
    int_array_append(calculation, 4);
    int_array_append(calculation, 3);
    int_array_append(calculation, 0);

    int a = find_registera_value(calc, calculation);

    assert_int_equal(a, 117440);
    
    free_array(calculation);
    free_array(calc->output);
    free(calc);
}

static void test_array_slice(void **state) {
    IntArray *arr = init_int_array(10);
    for (int i = 0; i < 10; i++) {
        int_array_append(arr, i);
    }

    IntArray *sliced = int_array_slice(arr, 4, 8);
    assert_int_equal(sliced->length, 4);
    for (int i = 0; i < 4; i++) {
        assert_true(arr->values[i + 4] == sliced->values[i]);
    }
    free_array(arr);
    free_array(sliced);
}

static void test_int_array_equal(void **state) {
    IntArray *arr = init_int_array(10);
    for (int i = 0; i < 10; i++) {
        int_array_append(arr, i);
    }

    IntArray *sliced = int_array_slice(arr, 4, 8);

    assert_true(int_array_equal(arr, arr));
    assert_false(int_array_equal(arr, sliced));
    free_array(arr);
    free_array(sliced);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_first_calculation),
        cmocka_unit_test(test_second_calculation),
        cmocka_unit_test(test_third_calculation),
        cmocka_unit_test(test_fourth_calculation),
        cmocka_unit_test(test_fifth_calculation),
        cmocka_unit_test(test_sixth_calculation),
        cmocka_unit_test(test_seventh_calculation),
        cmocka_unit_test(test_find_register_value),
        cmocka_unit_test(test_array_slice),
        cmocka_unit_test(test_int_array_equal),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);

    return 0;
}
