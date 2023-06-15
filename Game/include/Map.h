/*
** EPITECH PROJECT, 2021
** Map.h
** File description:
** Map struct and functions
*/

#ifndef MAP_H
    #define MAP_H

    #include "Enum.h"
    #include "Constants.h"
    #include "Player.h"

    typedef struct {
        int width;
        int height;
        Ressource** map;
        float density_food;
        float density_linemate;
        float density_deraumere;
        float density_sibur;
        float density_mendiane;
        float density_phiras;
        float density_thystame;
    } Map;

    Map* create_map(int width, int height);
    void generate_map(Map* map, Player** players, int num_players);
    void regenerate_ressources(Map* map, Player** players, int num_players);
    Ressource *generate_ressources(Map* map, int* tile_size);
    Color get_predominant_color(Ressource* tile, int tile_size);
    Color get_color(Ressource resource);
    void update_map(Map* map, Point point, Ressource ressource);
    Ressource get_tile(Map* map, Point point);
    void destroy_map(Map* map);
    void draw_map(Map* map, const char* filename, Player** players, int num_players);

#endif