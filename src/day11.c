#include <pthread.h>
#include <bits/pthreadtypes.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <threads.h>
#include "aoc.h"
#include "day11.h"
#define DIGIT_COUNT_METHOD 1


/*
Runtime Solution 1 (For 25 Iterations)
--------------------------------------
Total: 35.769000ms
--------------------------------------

Runtime Solution 2 (Using Hash Table)
-------------------------------------
Total                 : 814.93ms
Part 1 (25 Iterations): 4.88ms
Part 2 (75 Iterations): 810.03ms
-------------------------------------
*/

#if DIGIT_COUNT_METHOD == 1
// Count digits using maths
int count_digits(long num) {
    return floor(log10(num)) + 1;
}
#else
// Count digits using strings
int count_digits(u_long num) {
    char *str_num = malloc(sizeof(char) * 20);
    sprintf(str_num, "%ld", num);
    int len = strlen(str_num);
    free(str_num);
    return len;
}
#endif

HashMap *cache;

void init() {
    cache = init_map(1000);
}

LongArray *get_numbers(char *input) {
    LongArray *nums = init_long_array(10);
    char *cur = calloc(sizeof(char), 20);
    int cur_len = 0;
    for (int i = 0; i < strlen(input); i++) {
        if (input[i] != ' ') {
            cur[cur_len++] = input[i];
        } else {
            long_array_append(nums, atol(cur));
            memset(cur, 0, 20);
            cur_len = 0;
        }
    }
    if (cur_len != 0) {
        long_array_append(nums, atol(cur));
    }

    free(cur);

    return nums;
}

u_long number_blink(u_long num, int num_executions) {
    char *hash_key = malloc(sizeof(char) * 50);
    sprintf(hash_key, "%ld_%d", num, num_executions);
    if (map_has(cache, hash_key)) {
        u_long num_splits = map_long_get(cache, hash_key);
        return num_splits;
    }
    u_long num_splits = 0;
    if (num == 0) {
        num = 2024;
        if (num_executions > 1) {
            num_splits += number_blink(num, num_executions - 2);
        }
    } else {
        int len = count_digits(num);
        if (len % 2 == 0) {
            num_splits++;
            if (num_executions > 0) {
                u_long left_l, right_l;
                int factor = pow(10, (float) len / 2);

                left_l = num / factor;
                right_l = num % factor;
                num_splits += number_blink(left_l, num_executions - 1);
                num_splits += number_blink(right_l, num_executions - 1);
            }
        } else {
            if (num_executions > 0) {
                num_splits += number_blink(num * 2024, num_executions - 1);
            }
        }
    }

    map_long_put(cache, hash_key, num_splits);

    return num_splits;
}

u_long execute(char *input, int times) {
    LongArray *next = get_numbers(input);
    u_long num_splits = next->length;
    for (int i = 0; i < next->length; i++) {
        num_splits += number_blink(next->values[i], times - 1);
    }

    free_long_array(next);

    return num_splits;
}

int solve_day11(const char *input) {
    init();
    char *nums = read_file(input);

    Perf *perf = perf_init();
    timer_start("Total", perf);
    int t1 = timer_start("Part 1", perf);
    int p1 = execute(nums, 25);
    timer_stop(t1, perf);
    printf("Part 1: %d\n", p1);
    int t2 = timer_start("75", perf);
    u_long p2 = execute(nums, 75);
    timer_stop(t2, perf);
    printf("Part 2: %ld\n", p2);
    perf_report(perf);
    perf_close(perf);

    free(nums);
    free_map(cache);


    return 0;
}
