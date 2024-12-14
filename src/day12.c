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
} Area;

typedef struct AreaList {
    int max_length;
    int length;
    Area **areas;
} AreaList;

void free_area_list(AreaList *areas) {
    for (int i = 0; i < areas->length; i++) {
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

void count_sides_recursive(Area *area, IntMatrix *map, IntMatrix *visited, int cur_x, int cur_y, int offset_x, int offset_y) {
    int new_x = cur_x + offset_x;
    int new_y = cur_y + offset_y;
    visited->data[new_y][new_x] = 1;

    if (new_x == area->start_x && new_y == area->start_y) {
        return;
    } else {
        int next_x = new_x + offset_x;
        int next_y = new_y + offset_y;

        if (get_character(map, next_y, next_x) == area->identifier && visited->data[next_y][next_x] != 1) {
            count_sides_recursive(area, map, visited, new_x, new_y, offset_x, offset_y);
        }
        if (offset_y != 1 && get_character(map, new_y + 1, new_x) == area->identifier && visited->data[new_y + 1][new_x] != 1) {
            area->num_sides++;
            int next_offset_y = 1;
            int next_offset_x = 0;
            if (next_offset_y != area->start_y && next_offset_x != area->start_x) {
                count_sides_recursive(area, map, visited, new_x, new_y, next_offset_x, next_offset_y);
            }
        }
        if (offset_y != -1 && get_character(map, new_y - 1, new_x) == area->identifier && visited->data[new_y - 1][new_x] != 1) {
            area->num_sides++;
            int next_offset_y = -1;
            int next_offset_x = 0;
            if (next_offset_y != area->start_y && next_offset_x != area->start_x) {
                count_sides_recursive(area, map, visited, new_x, new_y, next_offset_x, next_offset_y);
            }
        }
        if (offset_x != 1 && get_character(map, new_y, new_x + 1) == area->identifier && visited->data[new_y][new_x + 1] != 1) {
            area->num_sides++;
            int next_offset_y = 0;
            int next_offset_x = 1;
            if (next_offset_y != area->start_y && next_offset_x != area->start_x) {
                count_sides_recursive(area, map, visited, new_x, new_y, next_offset_x, next_offset_y);
            }
        }
        if (offset_x != -1 && get_character(map, new_y, new_x - 1) == area->identifier && visited->data[new_y][new_x - 1] != 1) {
            area->num_sides++;
            int next_offset_y = 0;
            int next_offset_x = -1;
            if (!(next_offset_y == area->start_y && next_offset_x == area->start_x)) {
                count_sides_recursive(area, map, visited, new_x, new_y, next_offset_x, next_offset_y);
            }
        }
    }
}

void count_sides(Area *area, IntMatrix *map) {
    if (area->area_size <= 2) {
        area->num_sides = 4;
    } else {
        IntMatrix *visited = init_int_matrix(map->rows, map->cols);
        for (int i = 0; i < map->rows; i++) {
            for (int j = 0; j < map->cols; j++) {
                visited->data[i][j] = 0;
            }
        }
        visited->data[area->start_y][area->start_x] = 1;
        area->num_sides = 1;
        if (get_character(map, area->start_y + 1, area->start_x) == area->identifier) {
            count_sides_recursive(area, map, visited, area->start_x, area->start_y, 0, 1);
        }
        if (get_character(map, area->start_y - 1, area->start_x) == area->identifier) {
            count_sides_recursive(area, map, visited, area->start_x, area->start_y, 0, -1);
        }
        if (get_character(map, area->start_y, area->start_x + 1) == area->identifier) {
            count_sides_recursive(area, map, visited, area->start_x, area->start_y, 1, 0);
        }
        if (get_character(map, area->start_y, area->start_x - 1) == area->identifier) {
            count_sides_recursive(area, map, visited, area->start_x, area->start_y, -1, 0);
        }
        printf("Visited for %c\n", area->identifier);
        print_matrix(visited);
        free_matrix(visited);
    }
}

long calculate_price_sides(AreaList *list) {
    long price = 0;
    for (int i = 0; i < list->length; i++) {
        Area *a = list->areas[i];
        price += a->num_sides * a->area_size;
    }

    return price;
}

IntMatrix *extract_borders(IntMatrix *map) {
    IntMatrix *out = init_int_matrix(map->rows, map->cols);
    char *compare = calloc(sizeof(char), 5);
    char *expect = calloc(sizeof(char), 5);
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->cols; j++) {
            char *s = malloc(sizeof(char) * 2);
            char c = map->data[i][j];
            sprintf(s, "%c", c);

            char u = get_character(map, i - 1, j);
            char d = get_character(map, i + 1, j);
            char l = get_character(map, i, j - 1);
            char r = get_character(map, i, j + 1);

            int required_matches = 4;

            sprintf(compare, "%c%c%c%c", u, d, l, r);
            sprintf(expect, "%c%c%c%c", c, c, c, c);
            int difference = 0;
            for (int x = 0; x < 4; x++) if (compare[x] != expect[x]) difference++;

            if (4 - difference < required_matches) {
                out->data[i][j] = c;
                int cur_fence_count = 0;
            } else {
                out->data[i][j] = '.';
            }
        }
    }

    return out;
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
                printf("Area %c of size %ld with %ld fences\n", area->identifier, area->area_size, area->fence_count);
                area_list_append(areas, area);
            }
        }
    }

    timer_stop(t1, perf);

    IntMatrix *borders = extract_borders(map);
    print_matrix_as_char(borders);
    for (int i = 0; i < areas->length; i++) {
        count_sides(areas->areas[i], borders);
    }

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
