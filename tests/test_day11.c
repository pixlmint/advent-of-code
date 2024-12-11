#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include "aoc.h"
#include "day11.h"

static void test_get_numbers(void **state) {
    char *input = "123 333 12333 ";
    LongArray *arr = get_numbers(input);
    assert_int_equal(arr->values[0], 123);
    assert_int_equal(arr->values[1], 333);
    assert_int_equal(arr->values[2], 12333);
    assert_int_equal(arr->length, 3);

    free_long_array(arr);
}

static void test_blink(void **state) {
    assert_int_equal(number_blink(125, 0), 0);
    assert_int_equal(number_blink(17, 0), 1);
    assert_int_equal(number_blink(1234, 0), 1);
}

static void test_blink_complete(void **state) {
    char *input = "125 17";
    int out = execute(input, 25);
    assert_int_equal(out, 55312);
}

static void test_map_simple(void **state) {
    HashMap *map = init_map(10);

    char *key1 = "hello";
    long val1 = 1;
    char *key2 = "maps";
    int val2 = 2;

    map_long_put(map, key1, val1);
    long ret1 = map_long_get(map, key1);
    assert_int_equal(ret1, 1);

    free_map(map);
}

static void test_map_has(void **state) {
    HashMap *map = init_map(10);

    char *key1 = "hello";
    long val1 = 1;

    map_long_put(map, key1, val1);

    assert_true(map_has(map, key1));
    assert_false(map_has(map, "world"));
}

int main() {
    init();
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_map_simple),
        cmocka_unit_test(test_map_has),
        cmocka_unit_test(test_get_numbers),
        cmocka_unit_test(test_blink),
        cmocka_unit_test(test_blink_complete),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);

    return 0;
}
