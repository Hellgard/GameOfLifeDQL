/*
** EPITECH PROJECT, 2021
** Player.h
** File description:
** Player
*/

#ifndef PLAYER_H
    #define PLAYER_H

    #include "Enum.h"
    #include "Constants.h"
    #include "Inventory.h"

    typedef struct Player_t {
        int levelRequirements[7][7];
        int playerRequirements[7];
        int width;
        int height;
        Point position;
        Direction direction;
        Inventory inventory;
        struct Player_t *childs;
        int level;
        int id;
        int timeBeforeDeath;
        int appearance_time;
    } Player;

    void init_player(Player* player, int width, int height, int rand, int level);
    void play_sound(const char* sound_file);
    int get_distance(Player* player, Player* other_player);
    int check_evolution_requirements(Player* player, Player** players, int num_players);
    int can_evolve(Player* player);
    int get_id(Player* player);
    void move(Player* player);
    Inventory* get_inventory(Player* player);
    int get_level(Player* player);
    Direction get_direction(Player* player);
    Point get_position(Player* player);
    Point get_position_after(Player* player, Direction direction);
    void set_direction(Player* player, Direction direction);
    void set_position(Player* player, Point position);
    void set_level(Player* player, int level);
    void evolve(Player* player);
    void set_child(Player* player, Player* child);
    Player* get_childs(Player* player);
    Player* get_child(Player* player, int index);
    void show_player(Player* player);

#endif