#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "day14.h"
#include "aoc.h"

void free_robot(Robot *bot) {
    free(bot->starting_point);
    free(bot->velocity);
    free(bot->end_point);
    free(bot);
}

void free_robot_list(RobotList *list) {
    for (int i = 0; i < list->length; i++) {
        free_robot(list->robots[i]);
    }
    free(list->robots);
    free(list);
}

Robot *parse_robot(char *line) {
    Robot *bot = malloc(sizeof(Robot));
    char *cur_num = calloc(20, sizeof(char));
    bot->starting_point = malloc(sizeof(Point));
    bot->velocity = malloc(sizeof(Point));
    bot->end_point = malloc(sizeof(Point));
    int cur_num_len = 0;
    bool s_read = false;

    for (int i = 0; i < strlen(line); i++) {
        char c = line[i];
        if (c == 'v') {
            s_read = true;
        } else if (c == ',' || c == ' ' || c == '\n') {
            int num = atoi(cur_num);
            Point *p;
            if (s_read) p = bot->velocity;
            if (!s_read) p = bot->starting_point;
            if (c == ',') {
                p->x = num;
            } else {
                p->y = num;
            }
            memset(cur_num, 0, 20);
            cur_num_len = 0;
        } else if (is_numeric(c) || c == '-') {
            cur_num[cur_num_len++] = c;
        }
    }
    free(cur_num);
    bot->end_point->x = bot->starting_point->x;
    bot->end_point->y = bot->starting_point->y;

    return bot;
}

u_int euclidian_modulo(int a, int b) {
    return fmod(fmod(a, b) + b, b);
}

void execute_steps(const Robot *robot, const Point *bounds, const u_int num_steps) {
    /*robot->end_point->x = fmod(robot->starting_point->x + v_copy->x, bounds->x);*/
    /*robot->end_point->y = fmod(robot->starting_point->y + v_copy->y, bounds->y);*/
    /*if (robot->end_point->x < 0) {*/
    /*    robot->end_point->x = bounds->x + robot->end_point->x;*/
    /*}*/
    /*if (robot->end_point->y < 0) {*/
    /*    robot->end_point->y = bounds->y + robot->end_point->y;*/
    /*}*/
    robot->end_point->x = euclidian_modulo(robot->starting_point->x + (robot->velocity->x * num_steps), bounds->x);
    robot->end_point->y = euclidian_modulo(robot->starting_point->y + (robot->velocity->y * num_steps), bounds->y);
}

int get_quadrant(const Point *position, const Point *bounds) {
    Point center = {.x = floor(bounds->x / 2), .y = floor(bounds->y / 2)};
    // printf("Center: (%f, %f)\n", center.x, center.y);

    if (position->x < center.x && position->y < center.y) {
        return 0;
    }
    if (position->x < center.x && position->y > center.y) {
        return 1;
    }
    if (position->x > center.x && position->y < center.y) {
        return 2;
    }
    if (position->x > center.x && position->y > center.y) {
        return 3;
    }

    return -1;
}

IntMatrix *generate_bot_matrix(const RobotList *bots, const Point *bounds) {
    IntMatrix *map = init_int_matrix(bounds->y, bounds->x);
    for (int i = 0; i < bounds->y; i++) {
        for (int j = 0; j < bounds->x; j++) {
            map->data[i][j] = 0;
        }
    }

    for (int i = 0; i < bots->length; i++) {
        const Robot *bot = bots->robots[i];
        map->data[(int) bot->end_point->y][(int) bot->end_point->x] += 1;
    }

    return map;
}

long sum_quadrants(FILE *file, const Point *bounds, const u_int num_steps) {
    RobotList *robots = parse_input(file);

    int *quadrant_sum = malloc(sizeof(int) * 4);
    quadrant_sum[0] = 0;
    quadrant_sum[1] = 0;
    quadrant_sum[2] = 0;
    quadrant_sum[3] = 0;
    long res = 1;
    for (int i = 0; i < robots->length; i++) {
        execute_steps(robots->robots[i], bounds, num_steps);
        int q = get_quadrant(robots->robots[i]->end_point, bounds);
        if (q != -1) {
            quadrant_sum[q] += 1;
        }
    }

    IntMatrix *map = generate_bot_matrix(robots, bounds);
    free_matrix(map);

    free_robot_list(robots);

    for (int i = 0; i < 4; i++) {
        if (quadrant_sum[i] != 0) {
            res *= quadrant_sum[i];
        }
    }
    free(quadrant_sum);

    return res;
}

RobotList *parse_input(FILE *file) {
    int robots_count = count_lines(file);
    RobotList *robots = malloc(sizeof(RobotList));
    robots->robots = malloc(sizeof(Robot) * robots_count);
    robots->length = robots_count;

    char *line = calloc(255, sizeof(char));

    for (int i = 0; i < robots_count; i++) {
        fgets(line, 255, file);
        if (strlen(line) == 0) {
            break;
        }

        robots->robots[i] = parse_robot(line);

        memset(line, 0, 255);
    }
    free(line);

    return robots;
}

int solve_day14(const char *input) {
    FILE *file = fopen(input, "r");
    Point bounds = {.x = 101, .y = 103};
    long res = sum_quadrants(file, &bounds, 100);
    printf("Part 1: %ld\n", res);

    fclose(file);
    return 0;
}
