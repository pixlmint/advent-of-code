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

void count_sides_recursive(Area *area, IntMatrix *map, IntMatrix *visited, int cur_x, int cur_y) {
    visited->data[cur_y + 1][cur_x + 1] = 1;

    // Check all adjacent positions
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // right, down, left, up

    for (int i = 0; i < 4; i++) {
        int next_x = cur_x + directions[i][0];
        int next_y = cur_y + directions[i][1];
        char next_char = get_character(map, next_y, next_x);

        // printf("next_x = %d, next_y = %d\n", next_x, next_y);
        // If we find our shape's character, count it as a side
        if (next_char == area->identifier) {
            area->num_sides++;

            // Look for the next position to move to
            for (int j = 0; j < 4; j++) {
                if (j != (i + 2) % 4) {  // Don't go backwards
                    int try_x = cur_x + directions[j][0];
                    int try_y = cur_y + directions[j][1];
                    char try_char = get_character(map, try_y, try_x);

                    // Move to any position that isn't our shape's character and hasn't been visited
                    if (try_char != area->identifier && visited->data[try_y + 1][try_x + 1] != 1) {
                        count_sides_recursive(area, map, visited, try_x, try_y);
                        // return;  // Exit after finding next move
                    }
                }
            }
        }
        // If the next position isn't our character and hasn't been visited, we can move there
        else if (next_y >= 0 && next_x >= 0 && next_y < map->rows && next_x < map->cols && visited->data[next_y + 1][next_x + 1] != 1) {
            for (int j = 0; j < 4; j++) {
                if (j != (i + 2) % 4) {
                    int neightbor_x = next_x + directions[j][0];
                    int neighbor_y = next_y + directions[j][1];

                    if (get_character(map, neighbor_y, neightbor_x) == area->identifier) {
                        count_sides_recursive(area, map, visited, next_x, next_y);
                        return;  // Exit after moving
                    }
                }
            }
        }
    }
}

void count_sides(Area *area, IntMatrix *map) {
    if (area->area_size <= 2) {
        area->num_sides = 4;
        return;
    }

    // Initialize visited matrix
    IntMatrix *visited = init_int_matrix(map->rows + 2, map->cols + 2);
    for (int i = 0; i < visited->rows; i++) {
        for (int j = 0; j < visited->cols; j++) {
            visited->data[i][j] = 0;
        }
    }

    area->num_sides = 0;

    // Find an empty space adjacent to our starting position
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    for (int i = 0; i < 4; i++) {
        int start_x = area->start_x + directions[i][0];
        int start_y = area->start_y + directions[i][1];

        if (get_character(map, start_y, start_x) != area->identifier) {
            count_sides_recursive(area, map, visited, start_x, start_y);
            break;
        }
    }

    printf("Visited for %c\n", area->identifier);
    print_matrix(visited);
    free_matrix(visited);
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
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    int corners[] = {0, 0, 0, 0};
    char *neighbors = malloc(sizeof(char) * 5);
    sprintf(neighbors, "....");
    int adjacent_neighbors = 0;
    for (int i = 0; i < 4; i++) {
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];
        char neighbor = get_character(map, ny, nx);
        neighbors[i] = neighbor;
        if (neighbor == area->identifier) {
            adjacent_neighbors++;
        }
    }

    int sides;
    // we're inside a line
    if (neighbors[0] == area->identifier && neighbors[2] == area->identifier || neighbors[1] == area->identifier && neighbors[3] == area->identifier) {
        sides = 0;
    } else {
        if (adjacent_neighbors == 0) {
            sides = 4;
        } else if (adjacent_neighbors == 1) {
            sides = 2;
        } else if (adjacent_neighbors == 2) {
            sides = 1;
        } else if (adjacent_neighbors == 3) {
            sides = 1;
        }
    }
    printf("x: %d, y: %d\t%s\tsides: %d\n", x, y, neighbors, sides);
    free(neighbors);
    return sides;
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
                printf("Area %c of size %ld with %ld fences and %ld sides\n", area->identifier, area->area_size, area->fence_count, area->num_sides);
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
