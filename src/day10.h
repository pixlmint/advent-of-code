#ifndef DAY10_H
#define DAY10_H

#include "aoc.h"
#include <stdio.h>

typedef struct PathNode {
    int value;
    int reached_count;
    struct PathNode *top;
    struct PathNode *bot;
    struct PathNode *left;
    struct PathNode *right;
} PathNode;

typedef struct NodeList {
    int length;
    int max_length;
    PathNode **nodes;
} NodeList;

typedef struct PathMatrix {
    int rows;
    int cols;
    PathNode ***nodes;
} PathMatrix;

int solve_day10(const char *input);
PathMatrix *read_map(FILE *file);
NodeList *get_nodes(PathMatrix *map);
PathMatrix *init_path_matrix(int rows, int cols);
int count_paths(NodeList *start_nodes, PathMatrix *map, bool only_unique);
void free_path_matrix(PathMatrix *mat);
void print_path_matrix(PathMatrix *mat);
void append_node(NodeList *list, PathNode *node);
PathNode *create_node(int value);
int node_list_index_of(NodeList *nodes, PathNode *search);
void free_node_list(NodeList *nl);

#endif
