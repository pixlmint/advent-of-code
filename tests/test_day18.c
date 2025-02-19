#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include "day18.h"
#include "aoc.h"

static void test_str_split(void **state) {
    char *input = "Hello\nWorld";

    StringArray *split = str_split(input, '\n');

    assert_int_equal(2, split->length);
    assert_string_equal("Hello", split->values[0]);
    assert_string_equal("World", split->values[1]);

    free_string_array(split);
}

static void test_str_split_empty(void **state) {
    char *input = "";

    StringArray *split = str_split(input, '\n');

    assert_int_equal(0, split->length);

    free_string_array(split);
}

static void test_str_split_comma(void **state) {
    char *input = "11,22";
    StringArray *split = str_split(input, ',');
    assert_int_equal(split->length, 2);
    assert_string_equal(split->values[0], "11");
    assert_string_equal(split->values[1], "22");
    free_string_array(split);
}

static void test_read_input(void **state) {
    char *input = "11,22\n33,44\n7,8";
    PointArray *points = read_input(input);

    assert_int_equal(points->length, 3);

    const Point *first = points->points[0];
    const Point *second = points->points[1];
    const Point *third = points->points[2];
    assert_int_equal(first->x, 11);
    assert_int_equal(first->y, 22);
    assert_int_equal(second->x, 33);
    assert_int_equal(second->y, 44);
    assert_int_equal(third->x, 7);
    assert_int_equal(third->y, 8);

    free_point_array(points);
}

static void test_count_min_steps(void **state) {
    char *input = read_file("tests/test_day18_input.txt");
    PointArray *points = read_input(input);
    free(input);
    IntMatrix *map = init_int_matrix_value(7, 7, 0);

    simulate_bytes_falling(map, points, 0, 12);

    // print_matrix(map);

    size_t steps = count_min_steps(map);
    // print_matrix(map);

    free_point_array(points);
    free_matrix(map);

    int exp = 22;
    if (steps != exp) {
        printf("Wrong step count. Expected: %d, actual: %ld\n", exp, steps);
    }

    assert_int_equal(exp, steps);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_str_split),
        cmocka_unit_test(test_str_split_empty),
        cmocka_unit_test(test_str_split_comma),
        cmocka_unit_test(test_read_input),
        cmocka_unit_test(test_count_min_steps),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);

    return 0;
}
