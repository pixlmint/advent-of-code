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

