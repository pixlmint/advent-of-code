#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include "aoc.h"


char *read_file(const char *fp) {
    FILE *file = fopen(fp, "r");
    if (!file) {
        perror("Error opening input file");
        return "";
    }

    // Get length of file
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *input = malloc(length + 1);
    if (!input) {
        perror("Memory allocation failed");
        fclose(file);
        return "";
    }

    fread(input, 1, length, file);
    fclose(file);
    input[length] = '\0';

    return input;
}


bool is_numeric(char character) {
    return character > 47 && character < 58;
}

int char_to_int(char character) {
    return character - 48;
}

int int_to_char(int number) {
    return number + 48;
}

int count_lines(FILE *file) {
    char line[1000];
    int lines = 0;

    while (fgets(line, sizeof(line), file)) {
        lines++;
    }
    fseek(file, 0, SEEK_SET);

    return lines;
}

int str_count_lines(char *str) {
    int count = 0;

    const size_t len = strlen(str);

    for (int i = 0; i < len; i++) {
        if (str[i] == '\n') {
            count++;
        }
    }

    if (str[0] != '\0' && str[strlen(str) - 1] != '\n') {
        count++;
    }

    return count;
}

StringArray *str_split(const char *input, const char separator) {
    const size_t len = strlen(input);
    int array_length = (int) sqrt(len);
    if (array_length == 0) {
        array_length = 2;
    }
    StringArray *ret = init_string_array(array_length);
    if (len == 0) {
        return ret;
    }
    size_t current_length = 0;
    char c;
    char *current_string = calloc(sizeof(char), len);
    current_string[0] = '\0';
    size_t str_count = 0;

    for (int i = 0; i < len; i++) {
        c = input[i];
        if (c != separator) {
            current_string[current_length++] = c;
            current_string[current_length + 1] = '\0';
        } else {
            char *new_string = malloc(sizeof(char) * (current_length + 1));
            memcpy(new_string, current_string, current_length);
            new_string[current_length] = '\0';
            string_array_append(ret, new_string);
            current_string[0] = '\0';
            current_length = 0;
        }
    }

    if (input[len - 1] != separator) {
        char *new_string = malloc(sizeof(char) * (current_length + 1));
        strncpy(new_string, current_string, current_length);
        new_string[current_length] = '\0';
        string_array_append(ret, new_string);
    }

    free(current_string);

    return ret;
}

/**
 * Helper function for `count_columns` to make sure
 * this doesn't break past 1000 column wide lines
 */
int _count_columns_maxstr(FILE *file, int max_strlen) {
    char *line = calloc(max_strlen, sizeof(char));
    fgets(line, max_strlen, file);
    int num_cols = strlen(line);
    fseek(file, 0, SEEK_SET);
    if (line[num_cols - 1] == '\n') {
        line[num_cols - 1] = '\0';
        num_cols--;
    }
    free(line);
    return num_cols;
}

int count_columns(FILE *file) {
    int maxlen = 1000;
    int count = _count_columns_maxstr(file, maxlen);

    while (maxlen * 0.95 <= count) {
        maxlen = maxlen * 1.5;
        count = _count_columns_maxstr(file, maxlen);
    }

    return count;
}

int min(int x, int y) {
    return y ^ ((x ^ y)) & -(x < y);
}

int max(int x, int y) {
    return x > y ? x : y;
}

int gcd(int a, int b) {
    a = abs(a);
    b = abs(b);
    int gcd;
    for(int i = 1; i <= a && i <= b; ++i) {
        // Checks if i is factor of both integers
        if(a %i == 0 && b %i == 0)
            gcd = i;
    }
    return gcd;
}

double pow(double base, double exponent) {
    double result = 1.0;
    while (exponent != 0) {
        result *= base;
        --exponent;
    }
    return result;
}

void swap(int* arr, int i, int j) {
    int i_val = arr[i];
    arr[i] = arr[j];
    arr[j] = i_val;
}

/**
 * Basic sorting using insert sort
 * @param arr the Array to sort
 */
void sort_integer_array(int* arr, int arr_size) {
    for (int i = 1; i < arr_size; i++) {
        int i_val = arr[i];
        int j;
        for (j = i; (j > 0 && arr[j - 1] > i_val); j--) {
            arr[j] = arr[j - 1];
        }
        arr[j] = i_val;
    }
}

u_long hash(char *str) {
    u_long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

// ------------------ Path Stuff -------------
/**
 * Recursively appends all Point elements to the path_tiles array
 * that make up the path with the lowest score
 */
bool get_tiles_in_path(PathNode *node, int min_score, PointArray *path_tiles) {
    if (node == NULL || !node->reaches_target  || get_lowest_score(node) > min_score) {
        return false;
    }
    Point *p = malloc(sizeof(Point));
    p->x = node->x;
    p->y = node->y;
    if (point_array_index_of(path_tiles, p) != -1) {
        free(p);
        return false;
    }

    point_array_append(path_tiles, p);
    free(p);

    if (get_tiles_in_path(node->l, min_score, path_tiles)) {
        return true;
    } else if (get_tiles_in_path(node->r, min_score, path_tiles)) {
        return true;
    } else {
        return get_tiles_in_path(node->o, min_score, path_tiles);
    }
}

int count_path_tiles(PathNode *root) {
    int score = get_lowest_score(root);

    PointArray *nodes = init_point_array(10);

    get_tiles_in_path(root, score, nodes);

    int count = nodes->length;
    free_point_array(nodes);
    return count;
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

PathNode *init_path_node(int x, int y, int score, PathNode *previous) {
    PathNode *node = malloc(sizeof(PathNode));
    node->prev = previous;
    node->x = x;
    node->y = y;
    node->value = score;
    node->o = NULL;
    node->l = NULL;
    node->r = NULL;
    node->reaches_target = false;

    return node;
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

