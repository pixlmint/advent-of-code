#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aoc.h"
#include "day12.h"

typedef struct Area {
    char identifier;
    long area_size;
    long fence_count;
    long num_sides;
    int start_x;
    int start_y;
    PointArray *corners;
} Area;

typedef struct AreaList {
    int max_length;
    int length;
    Area **areas;
} AreaList;

void free_area_list(AreaList *areas) {
    for (int i = 0; i < areas->length; i++) {
        free_point_array(areas->areas[i]->corners);
        free(areas->areas[i]);
    }
    free(areas->areas);
    free(areas);
}

AreaList *init_area_list(int length) {
    AreaList *list = malloc(sizeof(AreaList));
    list->areas = malloc(sizeof(Area) * length);
    list->length = 0;
    list->max_length = length;
    return list;
}

void area_list_append(AreaList *list, Area *area) {
    if (list->length >= list->max_length) {
        list->max_length *= 1.5;
        list->areas = realloc(list->areas, sizeof(Area) * list->max_length);
    }
    list->areas[list->length++] = area;
}

char get_character(IntMatrix *map, int y, int x) {
    if (y < 0 || y >= map->rows) {
        return '0';
    }
    if (x < 0 || x >= map->cols) {
        return '0';
    }
    return map->data[y][x];
}

long calculate_price_fences(AreaList *list) {
    long price = 0;
    for (int i = 0; i < list->length; i++) {
        Area *a = list->areas[i];
        price += a->fence_count * a->area_size;
    }

    return price;
}

long calculate_price_sides(AreaList *list) {
    long price = 0;
    for (int i = 0; i < list->length; i++) {
        Area *a = list->areas[i];
        price += a->num_sides * a->area_size;
    }

    return price;
}

int count_corners(IntMatrix *map, int x, int y, Area *area) {
    char cur = area->identifier;
    int corners = 0;

    int directions[4][2][2] = {{{0, -1}, {1, 0}}, {{1, 0}, {0, 1}}, {{0, 1}, {-1, 0}}, {{-1, 0}, {0, -1}}};

    for (int i = 0; i < 4; i++) {
        int dy1 = directions[i][0][0];
        int dx1 = directions[i][0][1];
        int dy2 = directions[i][1][0];
        int dx2 = directions[i][1][1];
        char left = get_character(map, y + dy1, x + dx1);
        char right = get_character(map, y + dy2, x + dx2);
        char mid = get_character(map, y + dy1 + dy2, x + dx1 + dx2);

        if ((left != cur && right != cur) || (left == cur && right == cur && mid != cur)) {
            corners++;
        }
    }

    return corners;
}

void walk_area_recursive(IntMatrix *map, IntMatrix *visited, int x, int y, Area *area) {
    if (x < 0 || x >= map->cols || y < 0 || y >= map->rows) {
        return;
    }
    if (visited->data[y][x] == 1) {
        return;
    }
    if (map->data[y][x] != area->identifier) {
        return;
    }
    char n = get_character(map, y - 1, x);
    char s = get_character(map, y + 1, x);
    char e = get_character(map, y, x - 1);
    char w = get_character(map, y, x + 1);

    if (n != area->identifier) area->fence_count++;
    if (s != area->identifier) area->fence_count++;
    if (e != area->identifier) area->fence_count++;
    if (w != area->identifier) area->fence_count++;

    visited->data[y][x] = 1;
    area->area_size++;

    area->num_sides += count_corners(map, x, y, area);

    walk_area_recursive(map, visited, x - 1, y, area);
    walk_area_recursive(map, visited, x + 1, y, area);
    walk_area_recursive(map, visited, x, y - 1, area);
    walk_area_recursive(map, visited, x, y + 1, area);
}

Area *walk_area(IntMatrix *map, IntMatrix *visited, int x, int y) {
    Area *area = malloc(sizeof(Area));
    area->area_size = 0;
    area->fence_count = 0;
    area->identifier = map->data[y][x];
    area->num_sides = 0;
    area->start_x = x;
    area->start_y = y;
    area->corners = init_point_array(10);
    walk_area_recursive(map, visited, x, y, area);

    return area;
}

IntMatrix *read_map(FILE *file) {
    const int rows = count_lines(file);
    const int cols = count_columns(file);
    IntMatrix *map = init_int_matrix(rows, cols);
    AreaList *areas = init_area_list(10);

    Perf *perf = perf_init();
    int t1 = timer_start("Mapper", perf);
    IntMatrix *visited = init_int_matrix(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            visited->data[i][j] = 0;

            char c = fgetc(file);
            if (c == '\n') c = fgetc(file);
            map->data[i][j] = c;
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (visited->data[i][j] == 0) {
                Area *area = walk_area(map, visited, j, i);
                printf("Area %c of size %ld with %ld corners\n", area->identifier, area->area_size, area->num_sides);
                area_list_append(areas, area);
            }
        }
    }

    timer_stop(t1, perf);

    // IntMatrix *borders = extract_borders(map);
    // print_matrix_as_char(borders);
    // for (int i = 0; i < areas->length; i++) {
    //     count_sides(areas->areas[i], borders);
    // }

    timer_start("Calculation", perf);
    printf("Part 1: %ld\n", calculate_price_fences(areas));
    printf("Part 2: %ld\n", calculate_price_sides(areas));
    perf_report(perf);
    perf_close(perf);

    free_area_list(areas);
    free_matrix(visited);

    return map;
}

int solve_day12(const char *input) {
    FILE *file = fopen(input, "r");

    IntMatrix *map = read_map(file);

    free_matrix(map);
    return 0;
}
