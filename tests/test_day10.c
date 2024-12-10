#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include "aoc.h"
#include "day10.h"

static void test_read_map(void **state) {
    FILE *file = fopen("tests/test_day10_input.txt", "r");
    
    PathMatrix *mat = read_map(file);

    free_path_matrix(mat);

    fclose(file);
}

static void test_get_start_nodes(void **state) {
    FILE *file = fopen("tests/test_day10_input.txt", "r");
    
    PathMatrix *mat = read_map(file);

    fclose(file);

    NodeList *start_nodes = get_nodes(mat);

    assert_int_equal(start_nodes->length, 9);

    free_path_matrix(mat);
    free_node_list(start_nodes);
}

static void test_get_nodes_simple(void **state) {
    PathMatrix *mat = init_path_matrix(1, 10);
    for (int i = 0; i < 10; i++) {
        PathNode *node = create_node(i);
        mat->nodes[0][i] = node;
    }

    NodeList *start_nodes = get_nodes(mat);

    assert_int_equal(start_nodes->length, 1);

    int count = count_paths(start_nodes, mat, true);
    assert_int_equal(count, 1);

    free_path_matrix(mat);
    free_node_list(start_nodes);
}

static void test_count_paths(void **state) {
    FILE *file = fopen("tests/test_day10_input.txt", "r");
    
    PathMatrix *mat = read_map(file);

    fclose(file);

    NodeList *start_nodes = get_nodes(mat);

    int count = count_paths(start_nodes, mat, true);
    assert_int_equal(count, 36);

    free_path_matrix(mat);
    free_node_list(start_nodes);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_read_map),
        cmocka_unit_test(test_get_start_nodes),
        cmocka_unit_test(test_count_paths),
        cmocka_unit_test(test_get_nodes_simple),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);

    return 0;
}
