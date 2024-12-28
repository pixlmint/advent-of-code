#ifndef DAY14_H
#define DAY14_H

#include "aoc.h"

typedef struct Robot {
    Point *starting_point;
    Point *end_point;
    Point *velocity;
} Robot;

typedef struct RobotList {
    int length;
    Robot **robots;
} RobotList;

int solve_day14(const char *input);
Robot *parse_robot(char *line);
void free_robot(Robot *bot);
RobotList *parse_input(FILE *file);
void free_robot_list(RobotList *list);
void execute_steps(const Robot *robot, const Point *bounds, const u_int num_steps);
long sum_quadrants(FILE *file, const Point *bounds, const u_int num_steps);
int get_quadrant(const Point *position, const Point *bounds);


#endif
