#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include "aoc.h"
#include "day12.h"

static void test_read_map(void **state) {
    char *fp = "tests/test_day12_input2.txt";
    FILE *file = fopen(fp, "r");

    IntMatrix *map = read_map(file);

    free_matrix(map);

    fclose(file);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_read_map),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);

    return 0;
}
