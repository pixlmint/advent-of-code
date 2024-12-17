#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <threads.h>
#include "day16.h"
#include "aoc.h"


/*
----------------------------
Search Paths: 12.9  s
Count Tiles:  13    ms
----------------------------
*/

void **read_map(FILE *file) {
    int rows = count_lines(file);
    int cols = count_columns(file);
    IntMatrix *map = init_int_matrix(rows, cols);
    PathNode *start = malloc(sizeof(PathNode));
    start->prev = NULL;
    Point *end = malloc(sizeof(Point));
    void **ret = malloc(sizeof(IntMatrix*) + sizeof(Point*) * 2);
    ret[0] = map;
    ret[1] = start;
    ret[2] = end;

    char c;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            c = fgetc(file);
            if (c == '\n') {
                c = fgetc(file);
            }
            if (c == '.') {
                c = '0';
            } else if (c == 'S') {
                start->x = j;
                start->y = i;
                c = '0';
            } else if (c == 'E') {
                end->x = j;
                end->y = i;
            }
            map->data[i][j] = c;
        }
    }

    start->x = start->x -1;

    return ret;
}

char val(IntMatrix *map, Point *point) {
    if (point->y < 0 || point->y >= map->rows) {
        return -1;
    }
    if (point->x < 0 || point->x >= map->cols) {
        return -1;
    }

    return map->data[(int) point->y][(int) point->x];
}

PathNode *find_parent_node(PathNode* current, int x, int y) {
    if (current->prev != NULL) {
        if (current->prev->x == x && current->prev->y == y) {
            return current->prev;
        } else {
            return find_parent_node(current->prev, x, y);
        }
    }

    return NULL;
}

void free_path_tree(PathNode* node) {
    if (node->o != NULL) {
        free_path_tree(node->o);
    }
    if (node->l != NULL) {
        free_path_tree(node->l);
    }
    if (node->r != NULL) {
        free_path_tree(node->r);
    }
    free(node);
}

void insert_path(IntMatrix *map, PathNode *node, int min_score) {
    if (get_lowest_score(node) > min_score) {
        return;
    }
    map->data[node->y][node->x] = map->data[node->y][node->x]+ '!';
    if (node->l != NULL && node->l->reaches_target) {
        insert_path(map, node->l, min_score);
    }
    if (node->r != NULL && node->r->reaches_target) {
        insert_path(map, node->r, min_score);
    }
    if (node->o != NULL && node->o->reaches_target) {
        insert_path(map, node->o, min_score);
    }
}

void print_path(IntMatrix *map, PathNode *root) {
    IntMatrix *clone = clone_int_matrix(map);

    for (int i = 0; i < clone->rows; i++) {
        for (int j = 0; j < clone->cols; j++) {
            if (clone->data[i][j] == '0') {
                clone->data[i][j] = '.';
            }
        }
    }

    int score = get_lowest_score(root);
    insert_path(clone, root, score);

    print_matrix_as_char(clone);
    free_matrix(clone);
}

PathNode *find_path(IntMatrix *map, IntMatrix *visited, PathNode *previous, Point *direction, Point *target, int current_score, int *lowest_score) {
    int new_y = previous->y + (int) direction->y;
    int new_x = previous->x + (int) direction->x;

    char cur = map->data[new_y][new_x];

    if (cur == '#') {
        return NULL;
    }

    PathNode *current = malloc(sizeof(PathNode));
    current->prev = previous;
    current->x = new_x;
    current->y = new_y;
    current->value = current_score;
    current->o = NULL;
    current->l = NULL;
    current->r = NULL;
    current->reaches_target = false;

    int previous_visit = visited->data[(int) current->y][(int) current->x];
    if (previous_visit + 1000 < current_score && previous_visit != 0) {
        // Were already here with lower score
        return current;
    } else if (target->x == current->x && target->y == current->y) {
        // Found Target
        *lowest_score = current_score;
        current->reaches_target = true;
        return current;
    } else if (current_score >= *lowest_score) {
        // Already bigger than lowest score
        return current;
    } else if (cur == -1) {
        // current->value = INT_MAX;
        return current;
    } else if (find_parent_node(previous, current->x, current->y) != NULL && previous->x != current->x && previous->y != current->y) {
        // We were already here in the current path
        return current;
    }

    Point *l = malloc(sizeof(Point));
    Point *r = malloc(sizeof(Point));

    if (direction->x == 0) {
        l->x = -1;
        r->x = 1;
        l->y = 0;
        r->y = 0;
    } else {
        l->x = 0;
        r->x = 0;
        l->y = 1;
        r->y = -1;
    }

    visited->data[(int) current->y][(int) current->x] = current_score;

    current->o = find_path(map, visited, current, direction, target, current_score + 1, lowest_score);
    current->l = find_path(map, visited, current, l, target, current_score + 1001, lowest_score);
    current->r = find_path(map, visited, current, r, target, current_score + 1001, lowest_score);

    if ((current->o != NULL && current->o->reaches_target) || (current->r != NULL && current->r->reaches_target) || (current->l != NULL && current->l->reaches_target)) {
        current->reaches_target = true;
    }

    free(r);
    free(l);

    return current;
}

void get_tiles_in_path(PathNode *node, int min_score, PointArray *path_tiles) {
    if (node == NULL || !node->reaches_target  || get_lowest_score(node) > min_score) {
        return;
    }
    Point *p = malloc(sizeof(Point));
    p->x = node->x;
    p->y = node->y;
    if (point_array_index_of(path_tiles, p) != -1) {
        free(p);
        return;
    }

    point_array_append(path_tiles, p);
    free(p);

    get_tiles_in_path(node->l, min_score, path_tiles);
    get_tiles_in_path(node->r, min_score, path_tiles);
    get_tiles_in_path(node->o, min_score, path_tiles);
}

int count_path_tiles(PathNode *root) {
    int score = get_lowest_score(root);

    PointArray *nodes = init_point_array(10);

    get_tiles_in_path(root, score, nodes);

    int count = nodes->length;
    free_point_array(nodes);
    return count;
}

PathNode *search_paths(FILE *file) {
    void **data = read_map(file);
    IntMatrix *map = data[0];
    PathNode *start = data[1];
    Point *end = data[2];
    free(data);

    Point *dir = malloc(sizeof(Point));
    dir->x = 1;
    dir->y = 0;
    IntMatrix *visited = init_int_matrix(map->rows, map->cols);
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; j++) {
            visited->data[i][j] = 0;
        }
    }

    int lowest_score = INT_MAX;

    PathNode *root = find_path(map, visited, start, dir, end, 0, &lowest_score);

    free(end);
    free(start);
    free(dir);

    free_matrix(map);
    free_matrix(visited);

    return root;
}

int get_lowest_score(PathNode *node) {
    int lowest = INT_MAX;

    if (node->r != NULL && node->r->reaches_target) {
        int l_r = get_lowest_score(node->r);
        if (l_r < lowest) {
            lowest = l_r;
        }
    }
    if (node->o != NULL && node->o->reaches_target) {
        int l_o = get_lowest_score(node->o);
        if (l_o < lowest) {
            lowest = l_o;
        }
    }
    if (node->l != NULL && node->l->reaches_target) {
        int l_l = get_lowest_score(node->l);
        if (l_l < lowest) {
            lowest = l_l;
        }
    }

    if (
        (node->l == NULL || !node->l->reaches_target) 
        && (node->o == NULL || !node->o->reaches_target)
        && (node->r == NULL || !node->r->reaches_target)
    ) {
        return node->value;
    }

    return lowest;
}

int solve_day16(const char *input) {
    FILE *file = fopen(input, "r");

    Perf *perf = perf_init();
    timer_start("Total", perf);
    int t1 = timer_start("Search Paths", perf);
    PathNode *root = search_paths(file);
    timer_stop(t1, perf);
    fclose(file);
    int lowest_score = get_lowest_score(root);

    printf("Part 1: %d\n", lowest_score);

    int t2 = timer_start("Count Tiles", perf);
    int tile_count = count_path_tiles(root);

    printf("Part 2: %d\n", tile_count);

    perf_report(perf);
    perf_close(perf);

    free_path_tree(root);

    return 0;
}

