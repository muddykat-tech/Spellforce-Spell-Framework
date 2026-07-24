#ifndef SCREENS_LOADER_H
#define SCREENS_LOADER_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_SCREEN_ENTRIES 128
#define SCREEN_MAP_NAME_LEN 128
#define SCREEN_MSB_NAME_LEN 128

typedef struct
{
    char map_name[SCREEN_MAP_NAME_LEN];
    char msb_file[SCREEN_MSB_NAME_LEN];
} ScreenEntry;

void load_screens_json();
const char *find_screen_for_map(const char *map_name);

#endif
