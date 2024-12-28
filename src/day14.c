#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include "day14.h"
#include "aoc.h"

// BMP header structures
#pragma pack(push, 1)
typedef struct {
    uint16_t type;              // Magic identifier: 0x4d42
    uint32_t size;              // File size in bytes
    uint16_t reserved1;         // Not used
    uint16_t reserved2;         // Not used
    uint32_t offset;            // Offset to image data in bytes
    uint32_t dib_header_size;   // DIB Header size in bytes
    int32_t  width_px;          // Width of the image
    int32_t  height_px;         // Height of the image
    uint16_t num_planes;        // Number of color planes
    uint16_t bits_per_pixel;    // Bits per pixel
    uint32_t compression;       // Compression type
    uint32_t image_size_bytes;  // Image size in bytes
    int32_t  x_resolution_ppm;  // Pixels per meter
    int32_t  y_resolution_ppm;  // Pixels per meter
    uint32_t num_colors;        // Number of colors
    uint32_t important_colors;  // Important colors
} BMPHeader;
#pragma pack(pop)

void matrix_to_bmp(IntMatrix *map, const char* filename) {
    // Calculate row padding - each row must be a multiple of 4 bytes
    int padding = (4 - (map->cols % 4)) % 4;
    int row_size = map->cols + padding;
    
    // Create and initialize the BMP header
    BMPHeader header = {
        .type = 0x4D42,                    // BM
        .size = sizeof(BMPHeader) + (256 * 4) + (row_size * map->rows),
        .reserved1 = 0,
        .reserved2 = 0,
        .offset = sizeof(BMPHeader) + (256 * 4),  // Header size + color palette size
        .dib_header_size = 40,
        .width_px = map->cols,
        .height_px = map->rows,
        .num_planes = 1,
        .bits_per_pixel = 8,              // 8 bits per pixel (grayscale)
        .compression = 0,                  // No compression
        .image_size_bytes = row_size * map->rows,
        .x_resolution_ppm = 2835,         // 72 DPI
        .y_resolution_ppm = 2835,         // 72 DPI
        .num_colors = 256,                // Using full 8-bit grayscale
        .important_colors = 0
    };

    FILE* file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error: Could not create file %s\n", filename);
        return;
    }

    // Write the header
    fwrite(&header, sizeof(BMPHeader), 1, file);

    // Write the color palette (grayscale)
    for (int i = 0; i < 256; i++) {
        uint8_t color[4] = {i, i, i, 0};  // R, G, B, reserved
        fwrite(color, sizeof(color), 1, file);
    }

    // Write the pixel data (bottom-up, padded rows)
    uint8_t* row = malloc(row_size);
    for (int y = map->rows - 1; y >= 0; y--) {  // BMP stores rows bottom-to-top
        // Convert row to bytes
        for (int x = 0; x < map->cols; x++) {
            row[x] = map->data[y][x] > 0 ? 0 : 255;  // 0 = black, 255 = white
        }
        // Add padding
        for (int p = 0; p < padding; p++) {
            row[map->cols + p] = 0;
        }
        fwrite(row, row_size, 1, file);
    }

    free(row);
    fclose(file);
}

void free_robot(Robot *bot) {
    free(bot->starting_point);
    free(bot->velocity);
    free(bot->end_point);
    free(bot);
}

void free_robot_list(RobotList *list) {
    for (int i = 0; i < list->length; i++) {
        free_robot(list->robots[i]);
    }
    free(list->robots);
    free(list);
}

Robot *parse_robot(char *line) {
    Robot *bot = malloc(sizeof(Robot));
    char *cur_num = calloc(20, sizeof(char));
    bot->starting_point = malloc(sizeof(Point));
    bot->velocity = malloc(sizeof(Point));
    bot->end_point = malloc(sizeof(Point));
    int cur_num_len = 0;
    bool s_read = false;

    for (int i = 0; i < strlen(line); i++) {
        char c = line[i];
        if (c == 'v') {
            s_read = true;
        } else if (c == ',' || c == ' ' || c == '\n') {
            int num = atoi(cur_num);
            Point *p;
            if (s_read) p = bot->velocity;
            if (!s_read) p = bot->starting_point;
            if (c == ',') {
                p->x = num;
            } else {
                p->y = num;
            }
            memset(cur_num, 0, 20);
            cur_num_len = 0;
        } else if (is_numeric(c) || c == '-') {
            cur_num[cur_num_len++] = c;
        }
    }
    free(cur_num);
    bot->end_point->x = bot->starting_point->x;
    bot->end_point->y = bot->starting_point->y;

    return bot;
}

u_int euclidian_modulo(int a, int b) {
    return fmod(fmod(a, b) + b, b);
}

void execute_steps(const Robot *robot, const Point *bounds, const u_int num_steps) {
    robot->end_point->x = euclidian_modulo(robot->starting_point->x + (robot->velocity->x * num_steps), bounds->x);
    robot->end_point->y = euclidian_modulo(robot->starting_point->y + (robot->velocity->y * num_steps), bounds->y);
}

int get_quadrant(const Point *position, const Point *bounds) {
    Point center = {.x = floor(bounds->x / 2), .y = floor(bounds->y / 2)};
    // printf("Center: (%f, %f)\n", center.x, center.y);

    if (position->x < center.x && position->y < center.y) {
        return 0;
    }
    if (position->x < center.x && position->y > center.y) {
        return 1;
    }
    if (position->x > center.x && position->y < center.y) {
        return 2;
    }
    if (position->x > center.x && position->y > center.y) {
        return 3;
    }

    return -1;
}

IntMatrix *generate_bot_matrix(const RobotList *bots, const Point *bounds) {
    IntMatrix *map = init_int_matrix(bounds->y, bounds->x);
    for (int i = 0; i < bounds->y; i++) {
        for (int j = 0; j < bounds->x; j++) {
            map->data[i][j] = 0;
        }
    }

    for (int i = 0; i < bots->length; i++) {
        const Robot *bot = bots->robots[i];
        map->data[(int) bot->end_point->y][(int) bot->end_point->x] += 1;
    }

    return map;
}

long sum_quadrants(FILE *file, const Point *bounds, const u_int num_steps) {
    RobotList *robots = parse_input(file);

    int *quadrant_sum = malloc(sizeof(int) * 4);
    quadrant_sum[0] = 0;
    quadrant_sum[1] = 0;
    quadrant_sum[2] = 0;
    quadrant_sum[3] = 0;
    long res = 1;
    for (int i = 0; i < robots->length; i++) {
        execute_steps(robots->robots[i], bounds, num_steps);
        int q = get_quadrant(robots->robots[i]->end_point, bounds);
        if (q != -1) {
            quadrant_sum[q] += 1;
        }
    }

    IntMatrix *map = generate_bot_matrix(robots, bounds);
    free_matrix(map);

    free_robot_list(robots);

    for (int i = 0; i < 4; i++) {
        if (quadrant_sum[i] != 0) {
            res *= quadrant_sum[i];
        }
    }
    free(quadrant_sum);

    return res;
}

RobotList *parse_input(FILE *file) {
    int robots_count = count_lines(file);
    RobotList *robots = malloc(sizeof(RobotList));
    robots->robots = malloc(sizeof(Robot) * robots_count);
    robots->length = robots_count;

    char *line = calloc(255, sizeof(char));

    for (int i = 0; i < robots_count; i++) {
        fgets(line, 255, file);
        if (strlen(line) == 0) {
            break;
        }

        robots->robots[i] = parse_robot(line);

        memset(line, 0, 255);
    }
    free(line);

    return robots;
}

char* zero_pad_int(int number, int width) {
    // Calculate how many digits the number has
    int num_digits = (number == 0) ? 1 : snprintf(NULL, 0, "%d", number);
    
    // Allocate memory for the padded string
    // +1 for null terminator, +1 for negative sign if needed
    char* result = (char*)malloc(width + 2);
    if (result == NULL) {
        return NULL;
    }
    
    // Format the number into a temporary buffer
    char temp[32];  // Large enough for any int
    snprintf(temp, sizeof(temp), "%d", number);
    
    // Calculate padding needed
    int padding = width - strlen(temp);
    
    // If padding is negative, just return the number as-is
    if (padding < 0) {
        strcpy(result, temp);
        return result;
    }
    
    // Add padding zeros
    memset(result, '0', padding);
    
    // Copy the actual number
    strcpy(result + padding, temp);
    
    return result;
}

void generate_images(FILE *file, Point *bounds, char *base_path, int iterations_start, int iterations_end) {
    RobotList *bots = parse_input(file);

    for (int i = iterations_start; i < iterations_end; i++) {
        for (int j = 0; j < bots->length; j++) {
            execute_steps(bots->robots[j], bounds, i);
        }

        char *fname = malloc(1000);
        char *iteration = zero_pad_int(i, floor(log10(iterations_end) + 1));
        sprintf(fname, "%s/img_%s.bmp", base_path, iteration);

        IntMatrix *map = generate_bot_matrix(bots, bounds);
        matrix_to_bmp(map, fname);
        free_matrix(map);
        free(fname);
    }
}

int solve_day14(const char *input) {
    FILE *file = fopen(input, "r");
    Point bounds = {.x = 101, .y = 103};
    long res = sum_quadrants(file, &bounds, 100);
    printf("Part 1: %ld\n", res);
    fseek(file, 0, SEEK_SET);

    char *base = "/home/christian/Bilder/aoc/2024/day14";
    generate_images(file, &bounds, base, 0, 50000);
    fclose(file);
    return 0;
}
