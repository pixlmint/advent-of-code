#include <stdlib.h>
#include "aoc.h"

// ------------- Int Array ----------------------------------------
IntArray *init_int_array(const int max_length) {
    IntArray *numbers = malloc(sizeof(IntArray));
    numbers->length = 0;
    numbers->max_length = max_length;
    numbers->values = malloc(sizeof(int) * max_length);

    return numbers;
}

void int_array_extend(IntArray *array) {
    array->max_length += array->max_length / 2;
    array->values = realloc(array->values, sizeof(int) * array->max_length);
}

int int_array_append(IntArray *array, int value) {
    if (array->length == array->max_length) {
        int_array_extend(array); 
    }
    int pos = array->length;

    array->values[pos] = value;
    array->length += 1;
    return pos;
}

int int_array_index_of(IntArray *array, int search) {
    for (int i = 0; i < array->length; i++) {
        if (array->values[i] == search) {
            return i;
        }
    }

    return -1;
}

void free_array(struct IntArray *array) {
    free(array->values);
    free(array);
}

int *copy_array_section(int *source, int start, int end) {
    if (start > end) {
        fprintf(stderr, "Start is bigger than end: %d > %d\n", start, end);
        return NULL;
    }
    int *copy = malloc(sizeof(int) * (end - start));
    for (int i = start; i < end; i++) {
        copy[i - start] = source[i];
    }

    return copy;
}

void print_int_array(IntArray *arr) {
    printf("IntArray (length %d): [", arr->length);
    for (int i = 0; i < arr->length; i++) {
        if (i != 0) {
            printf(", ");
        }
        printf("%d", arr->values[i]);
    }
    printf("]\n");
}

// ------------- Long Array ----------------------------------------
struct LongArray *init_long_array(const int max_length) {
    LongArray *numbers = malloc(sizeof(struct LongArray));
    numbers->length = 0;
    numbers->max_length = max_length;
    numbers->values = malloc(sizeof(long) * max_length);

    return numbers;
}

void long_array_extend(struct LongArray *array) {
    array->max_length += array->max_length / 2;
    array->values = realloc(array->values, sizeof(long) * array->max_length);
}

int long_array_append(struct LongArray *array, long value) {
    if (array->length == array->max_length) {
        long_array_extend(array); 
    }
    int pos = array->length;

    array->values[pos] = value;
    array->length += 1;
    return pos;
}

int long_array_index_of(LongArray *array, long search) {
    for (int i = 0; i < array->length; i++) {
        if (array->values[i] == search) {
            return i;
        }
    }

    return -1;
}

void free_long_array(LongArray *array) {
    free(array->values);
    free(array);
}

void print_long_array(LongArray *arr) {
    printf("LongArray (length %d): [", arr->length);
    for (int i = 0; i < arr->length; i++) {
        if (i != 0) {
            printf(", ");
        }
        printf("%ld", arr->values[i]);
    }
    printf("]\n");
}


// ------------- Point Arrays ----------------------------------------

PointArray *init_point_array(int max_length) {
    PointArray *arr = malloc(sizeof(PointArray));
    arr->max_length = max_length;
    arr->length = 0;
    arr->points = malloc(sizeof(Point *) * max_length);

    return arr;
}

void _point_array_extend(PointArray *array) {
    array->max_length += array->max_length / 2;
    array->points = realloc(array->points, sizeof(Point) * array->max_length);
}

int point_array_append(PointArray *arr, Point *point) {
    return append_coords(arr, point->x, point->y);
}

int append_coords(PointArray *arr, double x, double y) {
    if (arr->length == arr->max_length) {
        _point_array_extend(arr); 
    }
    int pos = arr->length;
    Point *p = malloc(sizeof(Point));
    p->x = x;
    p->y = y;
    arr->points[pos] = p;
    arr->length++;
    return pos;
}

int point_array_index_of(PointArray *arr, Point *search) {
    for (int i = 0; i < arr->length; i++) {
        Point *p = arr->points[i];
        if (p->x == search->x && p->y == search->y) {
            return i;
        }
    }

    return -1;
}

PointArray *point_array_clone(PointArray *orig) {
    PointArray *new = init_point_array(orig->max_length);
    for (int i = 0; i < orig->length; i++) {
        Point *np = malloc(sizeof(Point));
        np->x = orig->points[i]->x;
        np->y = orig->points[i]->y;
        new->points[i] = np;
    }

    return new;
}

PointArray *point_array_merge(PointArray *a, PointArray *b) {
    PointArray *merged = init_point_array(a->length + b->length);

    for (int i = 0; i < a->length; i++) {
        merged->points[i] = a->points[i];
        merged->length++;
    }

    for (int i = 0; i < b->length; i++) {
        Point *p = b->points[i];
        if (point_array_index_of(merged, p) == -1) {
            append_coords(merged, p->x, p->y);
        }
    }

    return merged;
}

void free_point_array(PointArray *arr) {
    for (int i = 0; i < arr->length; i++) {
        free(arr->points[i]);
    }
    free(arr->points);
    free(arr);
}

// ------------- Int Matrices ----------------------------------------

struct IntMatrix *init_int_matrix(const int rows, const int cols) {
    IntMatrix *m = malloc(sizeof(IntMatrix));
    m->cols = cols;
    m->rows = rows;
    m->data = malloc(rows * sizeof(int*));

    for (int i = 0; i < rows; i++) {
        m->data[i] = malloc(sizeof(int) * cols);
    }

    return m;
}

IntMatrix *transpose_int_matrix(IntMatrix *original) {
    IntMatrix *transposed = init_int_matrix(original->cols, original->rows);

    for (int i = 0; i < original->rows; i++) {
        for (int j = 0; j < original->cols; j++) {
            transposed->data[j][i] = original->data[i][j];
        }
    }

    return transposed;
}

IntMatrix *clone_int_matrix(IntMatrix *original) {
    IntMatrix *clone = init_int_matrix(original->rows, original->cols);

    for (int i = 0; i < original->rows; i++) {
        for (int j = 0; j < original->cols; j++) {
            clone->data[i][j] = original->data[i][j];
        }
    }

    return clone;
}

IntMatrix *flip_int_matrix(IntMatrix *original) {
    IntMatrix *flipped = init_int_matrix(original->rows, original->cols);

    for (int i = 0; i < original->rows; i++) {
        for (int j = 0; j < original->cols; j++) {
            flipped->data[i][j] = original->data[i][original->cols - j - 1];
        }
    }

    return flipped;
}

IntArray *get_diagonal_from_matrix(IntMatrix *matrix, int row, int col) {
    IntArray *arr = init_int_array(matrix->cols + matrix->rows);
    if (row == 0 && col == 0) {
        for (int i = col; i < min(matrix->cols, matrix->rows); i++) {
            int_array_append(arr, matrix->data[i - col][i]);
        }
    } else if (row == 0) {
        for (int i = col; i < matrix->cols; i++) {
            int_array_append(arr, matrix->data[i - col][i]);
        }
    } else if (col == 0) {
        for (int i = row; i < matrix->rows; i++) {
            int_array_append(arr, matrix->data[i][i - row]);
        }
    }

    return arr;
}

void print_matrix(IntMatrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        printf("| ");
        for (int j = 0; j < matrix->cols; j++) {
            printf("%d | ", matrix->data[i][j]);
            // printf("%d", matrix->data[i][j]);
        }
        printf("\n");
    }
}

void print_matrix_as_char(IntMatrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        // printf("| ");
        for (int j = 0; j < matrix->cols; j++) {
            printf("%c", matrix->data[i][j]);
        }
        printf("\n");
    }
}

void free_matrix(IntMatrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}
