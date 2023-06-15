/*
** EPITECH PROJECT, 2021
** Map.c
** File description:
** Map implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/Map.h"
#include "../include/Enum.h"
#include "../include/Constants.h"
#include "../include/Player.h"
#include "../include/Color.h"
#include <string.h>

Map* create_map(int width, int height) {
    Map* map = (Map*)malloc(sizeof(Map));
    if (map == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for map.\n");
        exit(1);
    }

    map->width = width;
    map->height = height;

    map->map = (Ressource**)malloc(height * sizeof(Ressource*));
    if (map->map == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for map data.\n");
        exit(1);
    }

    for (int i = 0; i < height; i++) {
        map->map[i] = (Ressource*)malloc(width * sizeof(Ressource));
        if (map->map[i] == NULL) {
            fprintf(stderr, "Error: Failed to allocate memory for map row.\n");
            exit(1);
        }
    }

    map->density_food = Density_food;
    map->density_linemate = Density_linemate;
    map->density_deraumere = Density_deraumere;
    map->density_sibur = Density_sibur;
    map->density_mendiane = Density_mendiane;
    map->density_phiras = Density_phiras;
    map->density_thystame = Density_thystame;

    return map;
}

void generate_map(Map* map, Player ** players, int num_players) {
    srand(time(NULL));

    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            int tile_size = 1;
            Ressource* tile = (Ressource*)malloc(tile_size * sizeof(Ressource));
            tile[0] = Ressource_EMPTY;
            int isPlayer = 0;
            for (int i = 0; i < num_players; i++) {
                if (players[i]->position.x == x && players[i]->position.y == y) {
                    // append Ressource_PLAYER to tile
                    tile_size++;
                    tile = (Ressource*)realloc(tile, tile_size * sizeof(Ressource));
                    tile[tile_size - 1] = Ressource_PLAYER;
                    isPlayer = 1;
                    break;
                }
            }
            if (!isPlayer) {
                tile = generate_ressources(map, &tile_size);
            }

            map->map[y][x] = *tile;
        }
    }
}


void regenerate_ressources(Map* map, Player** players, int num_players) {
    srand(time(NULL));
    int tile_size = 1;
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            int isPlayer = 0;
            Ressource* tile = &map->map[y][x];
            for (int i = 0; i < num_players; i++) {
                if (players[i]->position.x == x && players[i]->position.y == y) {
                    isPlayer = 1;
                    break;
                }
            }

            if (!isPlayer) {
                tile = generate_ressources(map, &tile_size);
            }
            map->map[y][x] = *tile;
        }
    }
}

Ressource *generate_ressources(Map* map, int* tile_size) {
    Ressource* tile = (Ressource*)malloc(map->width * map->height * sizeof(Ressource));
    *tile_size = 0;
    float densities[] = {
        map->density_food,
        map->density_linemate,
        map->density_deraumere,
        map->density_sibur,
        map->density_mendiane,
        map->density_phiras,
        map->density_thystame
    };
    int num_resources = sizeof(densities) / sizeof(float);

    // Determine if the tile should be empty
    float empty_probability = 1;
    for (int i = 0; i < num_resources; i++) {
        empty_probability -= densities[i];
    }

    if (rand() / (float)RAND_MAX <= empty_probability) {
        return;
    }

    for (int i = 0; i < num_resources; i++) {
        if (rand() / (float)RAND_MAX <= densities[i]) {
            tile[(*tile_size)++] = i + 1;
        }
    }
    return tile;
}

Color get_predominant_color(Ressource* tile, int tile_size) {
    srand(time(NULL));

    int count = tile_size;
    int* resource_counts = (int*)malloc(count * sizeof(int));
    if (resource_counts == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for resource_counts.\n");
        exit(1);
    }
    memset(resource_counts, 0, count * sizeof(int));

    for (int i = 0; i < tile_size; i++) {
        resource_counts[tile[i]]++;
    }

    int max_count = 0;
    Ressource predominant_resources[1] = {0};
    int num_predominant_resources = 0;

    for (int i = 0; i < count; i++) {
        if (resource_counts[i] > max_count) {
            max_count = resource_counts[i];
            predominant_resources[0] = i;
            num_predominant_resources = 1;
        } else if (resource_counts[i] == max_count) {
            predominant_resources[num_predominant_resources++] = i;
        }
    }

    Ressource predominant_resource;
    if (num_predominant_resources > 0) {
        predominant_resource = predominant_resources[rand() % num_predominant_resources];
    } else if (tile_size > 0) {
        predominant_resource = tile[0];
    } else {
        predominant_resource = Ressource_EMPTY;
    }
    free(resource_counts);
    return get_color(predominant_resource);
}


Color get_color(Ressource resource) {
    switch (resource) {
        case Ressource_FOOD:
            return Green;
        case Ressource_LIMEMATE:
            return Grey;
        case Ressource_DERAUMERE:
            return Brown;
        case Ressource_SIBUR:
            return Blue;
        case Ressource_MENDIANE:
            return Purple;
        case Ressource_PHIRAS:
            return Yellow;
        case Ressource_THYSTAME:
            return White;
        default:
            return Red;
    }
}

void update_map(Map* map, Point point, Ressource ressource) {
    if (point.x < map->width && point.y < map->height) {
        map->map[point.y][point.x] = ressource;
    } else if (point.x >= map->width) {
        map->map[point.y][0] = ressource;
    } else if (point.y >= map->height) {
        map->map[0][point.x] = ressource;
    } else {
        map->map[0][0] = ressource;
    }
}

Ressource get_tile(Map* map, Point point) {
    if (point.x < map->width && point.y < map->height) {
        return map->map[point.y][point.x];
    } else if (point.x >= map->width) {
        return map->map[point.y][0];
    } else if (point.y >= map->height) {
        return map->map[0][point.x];
    } else {
        return map->map[0][0];
    }
}

void destroy_map(Map* map) {
    for (int i = 0; i < map->height; i++) {
        free(map->map[i]);
    }
    free(map->map);
    free(map);
}

void draw_map(Map* map, const char* filename, Player** players, int num_players) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Failed to open file %s.\n", filename);
        exit(1);
    }
    printf("Drawing map to %s\n", filename);
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            Ressource* tile = map->map[y] + x;
            int tile_size = sizeof(map->map[y]) / sizeof(Ressource);
            
            int is_player_here = 0;
            Color color;
            for (int i = 0; i < tile_size; i++)
                if (tile[i] == Ressource_PLAYER) {
                    color.r = 0;
                    color.g = 0;
                    color.b = 0;
                    is_player_here = 1;
                }

            if (!is_player_here) {
                color = get_predominant_color(tile, tile_size);
            }
            // Write colored dot to file
            if (color.r == 0 && color.g == 0 && color.b == 0) {
                fprintf(file, "\u2593"); // Full block
            } else if (color.r == 255 && color.g == 0 && color.b == 0) {
                fprintf(file, "\u2588"); // Dark shade
            } else if (color.r == 0 && color.g == 255 && color.b == 0) {
                fprintf(file, "\u2591"); // Light shade
            } else if (color.r == 0 && color.g == 0 && color.b == 255) {
                fprintf(file, "\u2592"); // Medium shade
            } else {
                fprintf(file, ".");
            }
        }
        fprintf(file, "\n");
    }
    printf("Done drawing map to %s\n", filename);
    printf("Map saved to %s\n", filename);
}
