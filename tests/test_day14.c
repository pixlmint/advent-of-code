#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include "aoc.h"
#include "day14.h"

static void test_parse_robot(void **state) {
    char *input = "p=8,15 v=12,-27\n";
    Robot *bot = parse_robot(input);

    assert_int_equal(bot->starting_point->x, 8);
    assert_int_equal(bot->starting_point->y, 15);
    assert_int_equal(bot->velocity->x, 12);
    assert_int_equal(bot->velocity->y, -27);

    free_robot(bot);
}

static void test_parse_input(void **state) {
    FILE *file = fopen("tests/test_day14_input.txt", "r");
    RobotList *robots = parse_input(file);
    fclose(file);

    assert_int_equal(robots->length, 12);


    free_robot_list(robots);
}

static void test_execute_steps(void **state) {
    Robot *bot = parse_robot("p=0,0 v=1,0\n");
    Point bounds = {.x = 10, .y = 10};

    // Just in x direction
    execute_steps(bot, &bounds, 5);

    assert_int_equal(bot->end_point->x, 5);
    assert_int_equal(bot->end_point->y, 0);

    // Both x and y
    bot->velocity->x = 1;
    bot->velocity->y = 1;

    execute_steps(bot, &bounds, 5);

    assert_int_equal(bot->end_point->x, 5);
    assert_int_equal(bot->end_point->y, 5);


    // Wrap around
    bot->velocity->x = 1;
    bot->velocity->y = 0;

    execute_steps(bot, &bounds, 15);

    assert_int_equal(bot->end_point->x, 5);
    assert_int_equal(bot->end_point->y, 0);


    // Negative Velocity
    bot->velocity->x = -1;
    bot->velocity->y = 0;

    execute_steps(bot, &bounds, 4);

    assert_int_equal(bot->end_point->x, 6);
    assert_int_equal(bot->end_point->y, 0);

    free_robot(bot);
}

static void test_dummy(void **state) {
    FILE *file = fopen("tests/test_day14_input.txt", "r");

    Point bounds = {.x = 11, .y = 7};
    long res = sum_quadrants(file, &bounds, 100);
    fclose(file);

    // printf("Result: %ld\n", res);

    assert_int_equal(res, 12);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_parse_robot),
        cmocka_unit_test(test_parse_input),
        cmocka_unit_test(test_execute_steps),
        cmocka_unit_test(test_dummy),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);

    return 0;
}
