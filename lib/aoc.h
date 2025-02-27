#ifndef AOC
#define AOC
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>

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


// ------------- Hash Map -----------------------------------------
typedef struct HashMap {
    int length;
    int max_length;
    unsigned int *map_state;
    char **keys;
    void **values;
} HashMap;

HashMap *init_map(int length);
void map_put(HashMap *map, char *key, void *value);
void *map_get(HashMap *map, char *key);
bool map_has(HashMap *map, char *key);
void map_remove(HashMap *map, char *key);
long map_long_get(HashMap *map, char *key);
void map_long_put(HashMap *map, char *key, long value);
PathNode *map_node_get(HashMap *map, char *key);
void free_map(HashMap *map);
void print_map(HashMap *map);

// ------------- Int Array ----------------------------------------
typedef struct IntArray {
    int length;
    int max_length;
    int *values;
} IntArray;

void sort_integer_array(int* arr, int arr_size);
IntArray *init_int_array(const int max_length);
int int_array_append(IntArray *array, int value);
int int_array_index_of(IntArray *array, int search);
void free_array(IntArray *array);
void print_int_array(IntArray *arr);
int *copy_array_section(int* source, int start, int end);
IntArray *int_array_slice(IntArray *original, int start_i, int end_i);
void int_array_slice_to(IntArray *original, IntArray *target, int start_i, int end_i);
bool int_array_equal(IntArray *a, IntArray *b);

// ------------- Long Array ---------------------------------------
typedef struct LongArray {
    int length;
    int max_length;
    long *values;
} LongArray;
LongArray *init_long_array(const int max_length);
int long_array_append(LongArray *array, long value);
int long_array_index_of(LongArray *array, long search);
void free_long_array(LongArray *array);
void print_long_array(LongArray *arr);

// ------------- String Array -------------------------------------
typedef struct StringArray {
    int length;
    int max_length;
    char **values;
} StringArray;
StringArray *init_string_array(const int max_length);
int string_array_append(StringArray *array, char *value);
void print_string_array(StringArray *array);
void free_string_array(StringArray *array);

// ------------- Int Matrix ---------------------------------------
typedef struct IntMatrix {
    int **data;
    int rows;
    int cols;
} IntMatrix;

IntMatrix *init_int_matrix(const int rows, const int cols);
IntMatrix *init_int_matrix_value(const int rows, const int cols, const int value);
void free_matrix(IntMatrix *matrix);
IntMatrix *transpose_int_matrix(IntMatrix *original);
IntMatrix *flip_int_matrix(IntMatrix *original);
IntMatrix *clone_int_matrix(IntMatrix *original);
void print_matrix(IntMatrix *matrix);
void print_matrix_as_char(IntMatrix *matrix);
IntArray *get_diagonal_from_matrix(IntMatrix *matrix, int row, int col);
IntMatrix *str_to_matrix(char *str);

// ------------- Tuples ------------------------------------------
typedef struct PtrTuple {
    void *a;
    void *b;
} PtrTuple;

PtrTuple *init_tuple(void *a, void *b);


// ------------- Points ------------------------------------------
typedef struct Point {
    double x;
    double y;
} Point;

typedef struct PointArray {
    int length;
    int max_length;
    Point **points;
} PointArray;

PointArray *init_point_array(const int max_length);
int point_array_append(PointArray *arr, Point *point);
int append_coords(PointArray *arr, double x, double y);
int point_array_index_of(PointArray *arr, Point *search);
void free_point_array(PointArray *arr);
PointArray *point_array_merge(PointArray *a, PointArray *b);
PointArray *point_array_clone(PointArray *orig);
void print_point_array(PointArray *arr);

// ------------- Paths -------------------------------------------
#define MAPS_WALL '#'  // The Character used to identify non-path elements

void free_path_tree(PathNode* node);
int get_lowest_score(PathNode *node);
int count_path_tiles(PathNode *node);
PathNode *find_parent_node(PathNode* current, int x, int y);
PathNode *init_path_node(int x, int y, int score, PathNode *previous);
bool get_tiles_in_path(PathNode *node, int min_score, PointArray *path_tiles);
void print_path(IntMatrix *map, PathNode *root);

// ------------- File Handling ----------------------------------
char *read_file(const char *fp);
int count_lines(FILE *file);
int str_count_lines(char *str);
StringArray *str_split(const char *input, const char separator);
int count_columns(FILE *file);


// ------------- Other -----------------------------------------
int min(int x, int y);
double pow(double base, double exponent);
int gcd(int a, int b);

bool is_numeric(char character);
int char_to_int(char character);
int int_to_char(int number);
u_long hash(char *str);


// ------------- Perf -----------------------------------------

typedef struct Timer {
    clock_t start_time;
    clock_t stop_time;
    char *name;
} Timer;

typedef struct Perf {
    int length;
    int max_length;
    Timer **timers;
} Perf;

Perf *perf_init();
int timer_start(char *name, Perf *perf);
void timer_stop(int timer, Perf *perf);
void perf_report(Perf *perf);
void perf_close(Perf *perf);

#endif
