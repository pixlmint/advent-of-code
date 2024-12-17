#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include "day16.h"
#include "aoc.h"

static void test_get_score(void **state) {
    FILE *file = fopen("tests/test_day16_input.txt", "r");

    PathNode *root = search_paths(file);
    fclose(file);

    int lowest_score = get_lowest_score(root);

    free_path_tree(root);

    assert_int_equal(lowest_score, 11048);
}

static void test_count_path_tiles(void **state) {
    FILE *file = fopen("tests/test_day16_input.txt", "r");

    PathNode *root = search_paths(file);
    fclose(file);

    int tile_count = count_path_tiles(root);

    free_path_tree(root);

    assert_int_equal(tile_count, 64);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_get_score),
        cmocka_unit_test(test_count_path_tiles),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);

    return 0;
}
