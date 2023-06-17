#ifndef GAME_OF_LIFE_AI_H
    #define GAME_OF_LIFE_AI_H

    #include "Enum.h"
    #include "Player.h"
    #include "Map.h"
    #include <stdbool.h>

    typedef struct map_size {
        int x;
        int y;
    } map_size;

    typedef struct {
        map_size map_size;
        double speed;
        double time;
        int current;
        int timeBeforeRegen;
        Player** players;
        Map* map;
        int evolve_Iteration;
        Player* nearest;
        int nb_team;
        int nb_player_per_team;
        int *teams_slot;
        int frequency;
        char **teams_name;
    } GameOfLifeAI;

    GameOfLifeAI* create_game_of_life_ai(int width, int height, int nb_team, int nb_player_per_team, int frequency, char **teams);
    // void reset_game(GameOfLifeAI* game);
    bool check_evolution_player(GameOfLifeAI* game, Player* player);
    void add_egg(GameOfLifeAI* game, Player* player);
    void nearest_player_same_level(GameOfLifeAI* game, Player* player);
    void check_evolution(GameOfLifeAI* game);
    void add_pending_players(GameOfLifeAI* game);
    void add_player(GameOfLifeAI* game, Player* player);
    Player* get_player(GameOfLifeAI* game, int index);
    Ressource *get_resources(Map *map, Point position, map_size map_size);
    void check_user_input(Player* player);
    Player** get_players(GameOfLifeAI* game);
    Ressource *check_next_move(GameOfLifeAI* game, Player* player, Point* player_pos);
    bool verify_next_tile(GameOfLifeAI* game, Player* player, Point posNext, int* reward);
    int move_player(Player* player, int condition, Ressource* resources, Point player_pos);
    void check_game_state(GameOfLifeAI* game);
    int check_player_state(Player* player, int* reward);
    Direction choose_direction(int* action, Player* player);
    Inventory check_resources(Ressource* resources, Player* player);
#endif