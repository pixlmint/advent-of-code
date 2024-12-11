#include <stdlib.h>
#include "aoc.h"

const int MAP_STATE_FREE = 1;
const int MAP_STATE_USED = 2;
const int MAP_STATE_DELETED = 3;

HashMap *init_map(int length) {
    HashMap *map = malloc(sizeof(HashMap));
    map->length = 0;
    map->max_length = length;
    map->values = malloc(sizeof(int*) * length);
    map->keys = malloc(sizeof(char*) * length);
    map->map_state = malloc(sizeof(unsigned int) * length);

    for (int i = 0; i < length; i++) {
        map->map_state[i] = MAP_STATE_FREE;
    }

    return map;
}

int map_hash(char *key, HashMap *map) {
    unsigned long h = hash(key);
    return h % map->max_length;
}

int calc_position(HashMap *map, char *key) {
    int cur_pos = map_hash(key, map);
    int counter = 0;

    while (counter < map->max_length) {
        if (map->map_state[cur_pos] == MAP_STATE_FREE) {
            return cur_pos;
        }
        if (map->map_state[cur_pos] == MAP_STATE_USED && hash(map->keys[cur_pos]) == hash(key)) {
            return cur_pos;
        }
        cur_pos = (cur_pos + 1) % map->max_length;
        counter++;
    }

    return -1;
}

void *map_get(HashMap *map, char *key) {
    int index = calc_position(map, key);
    return index >= 0 ? map->values[index] : NULL;
}

void inflate_and_rehash(HashMap *map) {
    unsigned int *original_map_state = map->map_state;
    char **original_keys = map->keys;
    void **original_values = map->values;

    int new_length = map->max_length * 2;
    int original_length = map->max_length;
    // printf("Inflating from %d to %d (length: %d)\n", original_length, new_length, map->length);
    
    map->keys = malloc(sizeof(char*) * new_length);
    map->values = malloc(sizeof(int*) * new_length);
    map->map_state = malloc(sizeof(unsigned int) * new_length);

    for (int i = 0; i < new_length; i++) {
        map->map_state[i] = MAP_STATE_FREE;
    }
    map->length = 0;
    map->max_length = new_length;

    for (int i = 0; i < original_length; i++) {
        if (original_map_state[i] == MAP_STATE_USED) {
            map_put(map, original_keys[i], original_values[i]);
        }
    }

    free(original_keys);
    free(original_values);
    free(original_map_state);
    // printf("Done inflating\n");
}

void map_put(HashMap *map, char *key, void *value) {
    int key_index = calc_position(map, key);

    if (key_index < 0 || map->map_state[key_index] == MAP_STATE_FREE) {
        if (map->length > 0.75 * map->max_length || key_index < 0) {
            inflate_and_rehash(map);
            key_index = calc_position(map, key);
        }

        while (map->map_state[key_index] == MAP_STATE_USED) {
            key_index = (key_index + 1) % map->max_length;
        }

        map->length++;
        map->keys[key_index] = key;
        map->values[key_index] = value;
    } else {
        map->values[key_index] = value;
        map->keys[key_index] = key;
    }
    map->map_state[key_index] = MAP_STATE_USED;
}

void map_remove(HashMap *map, char *key) {
    int key_index = calc_position(map, key);
    if (key_index >= 0 && map->map_state[key_index] != MAP_STATE_FREE) {
        map->length -= 1;
        map->map_state[key_index] = MAP_STATE_DELETED;
    }
}

bool map_has(HashMap *map, char *key) {
    int index = calc_position(map, key);
    if (index < 0) {
        return false;
    }
    return map->map_state[index] == MAP_STATE_USED && hash(key) == hash(map->keys[index]);
}

long map_long_get(HashMap *map, char *key) {
    long *ptr = (long*) map_get(map, key);
    return ptr ? *ptr : 0;
}

void map_long_put(HashMap *map, char *key, long value) {
    long *value_ptr = malloc(sizeof(long));
    *value_ptr = value;
    map_put(map, key, value_ptr);
}

void free_map(HashMap *map) {
    for (int i = 0; i < map->max_length; i++) {
        if (map->map_state[i] == MAP_STATE_USED) {
            free(map->values[i]);
        }
    }
    free(map->values);
    free(map->keys);
    free(map->map_state);
    free(map);
}

void print_map(HashMap *map) {
    printf("HashMap of capacity %d and length %d\n", map->max_length, map->length);
    for (int i = 0; i < map->max_length; i++) {
        if (map->map_state[i] != MAP_STATE_FREE) {
            printf("(%d): %s -> %d\n", i, map->keys[i], *(int*) map->values[i]);
        }
    }
}

