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

    map->map = (Ressource***)malloc(height * sizeof(Ressource**));
    if (map->map == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for map->map.\n");
        exit(1);
    }

    for (int y = 0; y < height; y++) {
        map->map[y] = (Ressource**)malloc(width * sizeof(Ressource*));
        if (map->map[y] == NULL) {
            fprintf(stderr, "Error: Failed to allocate memory for map->map[%d].\n", y);
            exit(1);
        }
        for (int x = 0; x < width; x++) {
            map->map[y][x] = (Ressource*)malloc(sizeof(Ressource));
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

void generate_map(Map* map, Player **players, int num_players) {
    srand(time(NULL));
    printf("Generating map...\n");
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            int tile_size = 1;
            Ressource* tile = (Ressource*)malloc(tile_size * sizeof(Ressource));
            tile[0] = Ressource_EMPTY;
            int isPlayer = 0;
            for (int i = 0; i < num_players; i++) {  // Changed loop condition to `i < num_players`
                if (players[i]->position.x == x && players[i]->position.y == y) {
                    printf("Player found!\n");
                    tile_size++;
                    tile = (Ressource*)realloc(tile, tile_size * sizeof(Ressource));
                    tile[tile_size - 1] = Ressource_PLAYER;
                    isPlayer = 1;
                    break;
                }
            }
            if (!isPlayer) {
                free(tile);  // Free the initially allocated tile
                tile = generate_ressources(map, &tile_size);  // Update tile with generated resources
                printf("Resources generated!\n");
            }
            for (int i = 0; i < tile_size; i++) {
                printf("tile[%d] = %d\n", i, tile[i]);
            }
            map->map[y][x] = malloc(tile_size * sizeof(Ressource));
            map->map[y][x] = tile;
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
            map->map[y][x] = malloc(tile_size * sizeof(Ressource));
            map->map[y][x] = tile;
        }
    }
}

Ressource *generate_ressources(Map* map, int* tile_size) {
    *tile_size = rand() % 6;
    Ressource* tile = (Ressource*)malloc(*tile_size * sizeof(Ressource));
    float densities[] = {
        map->density_thystame,
        map->density_phiras,
        map->density_mendiane,
        map->density_sibur,
        map->density_deraumere,
        map->density_linemate,
        map->density_food
    };

    Ressource resources[] = {
        Ressource_THYSTAME,
        Ressource_PHIRAS,
        Ressource_MENDIANE,
        Ressource_SIBUR,
        Ressource_DERAUMERE,
        Ressource_LIMEMATE,
        Ressource_FOOD
    };
    int num_resources = *tile_size;

    // Determine if the tile should be empty
    float empty_probability = 1;
    for (int i = 0; i < num_resources; i++) {
        empty_probability -= densities[i];
    }

    if (rand() / (float)RAND_MAX <= empty_probability) {
        tile[0] = Ressource_EMPTY;
        return tile;
    }
    int j = 0;
    for (int i = 0; j < num_resources; i++) {
        if (rand() / (float)RAND_MAX <= densities[i]) {
            tile[j] = resources[i];
            j++;
        }
        if (i == 7 - 1) {
            i = 0;
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

void update_map(Map* map, Point point, Ressource *ressources) {
    if (point.x < map->width && point.y < map->height) {
        map->map[point.y][point.x] = ressources;
    } else if (point.x >= map->width) {
        map->map[point.y][0] = ressources;
    } else if (point.y >= map->height) {
        map->map[0][point.x] = ressources;
    } else {
        map->map[0][0] = ressources;
    }
}

Ressource *get_tile(Map* map, Point point) {
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
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            
            int is_player_here = 0;
            Color color;
            for (int j = 0; j < num_players; j++)
                if (players[j]->position.x == x && players[j]->position.y == y) {
                    is_player_here = 1;
                }

            if (is_player_here == 0) {
                fprintf(file, "\u2588"); // Full block
            } else {
                fprintf(file, "\u25CF"); // Black circle
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}
