#ifndef DAY16_H
#define DAY16_H
#include "aoc.h"

typedef struct PathNode {
    int value;
    int x;
    int y;
    bool reaches_target;
    struct PathNode *prev;
    struct PathNode *o;
    struct PathNode *r;
    struct PathNode *l;
} PathNode;

void **read_map(FILE *file);
int solve_day16(const char *input);
PathNode *find_path(IntMatrix *map, IntMatrix *visited, PathNode *previous, Point *direction, Point *target, int current_score, int *lowest_score);
void free_path_tree(PathNode* node);
void print_path(IntMatrix *map, PathNode *root);
int get_lowest_score(PathNode *node);
PathNode *search_paths(FILE *file);

#endif
