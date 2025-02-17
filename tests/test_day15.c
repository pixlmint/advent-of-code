#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include "day15.h"
#include "aoc.h"

static void test_str_count_lines(void **state) {
    char *empty = "";
    char *single = "Hello World";
    char *singleE = "Hello World\n";
    char *two = "Hello\nWorld";
    char *twoE = "Hello\nWorld\n";
    assert_int_equal(0, str_count_lines(empty));
    assert_int_equal(1, str_count_lines(single));
    assert_int_equal(1, str_count_lines(singleE));
    assert_int_equal(2, str_count_lines(two));
    assert_int_equal(2, str_count_lines(twoE));
}

static void test_str_to_matrix(void **state) {
    char *input = "123\n456\n789";
    IntMatrix *matrix = str_to_matrix(input);
    assert_int_equal(3, matrix->rows);
    assert_int_equal(3, matrix->cols);
    free_matrix(matrix);
}

static void test_part1(void **state) {
    char *input = "tests/test_day15_input.txt";
    int result = part_1(input);
    int exp = 2028;
    if (result != exp) {
        printf("Result: %d\n", result);
    }
    assert_int_equal(result, exp);
}

static void test_part1_large(void **state) {
    char *input = "tests/test_day15_input2.txt";
    int exp = 10092;
    int result = part_1(input);
    if (result != exp) {
        printf("Result: %d\n", result);
    }
    assert_int_equal(result, exp);
}

static void test_part2(void **state) {
    char *input = "tests/test_day15_input3.txt";
    int result = part_2(input);
    int exp = 618;
    if (result != exp) {
        printf("Result: %d\n", result);
    }
    assert_int_equal(result, exp);
}

static void test_part2_large(void **state) {
    char *input = "tests/test_day15_input2.txt";
    int result = part_2(input);
    int exp = 9021;
    if (result != exp) {
        printf("Result: %d\n", result);
    }
    assert_int_equal(result, exp);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_str_count_lines),
        cmocka_unit_test(test_str_to_matrix),
        cmocka_unit_test(test_part1),
        cmocka_unit_test(test_part1_large),
        cmocka_unit_test(test_part2),
        cmocka_unit_test(test_part2_large),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);

    return 0;
}
