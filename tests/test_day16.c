#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include "day16.h"
#include "aoc.h"

static void test_dummy(void **state) {
    FILE *file = fopen("tests/test_day16_input.txt", "r");

    void **data = read_map(file);
    IntMatrix *map = data[0];
    Point *start = data[1];
    Point *end = data[2];
    free(data);
    fclose(file);

    PointArray *prev = init_point_array(10);
    Point *dir = malloc(sizeof(Point));
    dir->x = 1;
    dir->y = 0;
    int best_score = find_path(map, prev, start, dir, end, 0);

    print_matrix_as_char(map);

    printf("Score: %d\n", best_score);
    assert_int_equal(best_score, 11048);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dummy),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);

    return 0;
}
