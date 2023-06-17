#include "../include/Game.h"
#include "../include/Player.h"
#include "../include/Map.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
// rand is included in stdlib.h

Player **init_players(Player **players, int nb_team, int nb_player_per_team, int width, int height, char **teams_name)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int random = 0;
    int level = 1;
    int id = 0;
    int nb_player = nb_team * nb_player_per_team;

    srand(time(NULL));
    for (i = 0; i < nb_team; i++)
    {
        for (j = 0; j < nb_player_per_team; j++)
        {
            random = rand() % 4;
            players[k] = init_player(width, height, random, level, teams_name[i]);
            players[k]->width = width;
            players[k]->height = height;
            k++;
        }
    }
    
    players[k] = NONE;
    return players;
}

GameOfLifeAI* create_game_of_life_ai(int width, int height, int nb_team, int nb_player_per_team, int frequency, char **teams_name)
{
    GameOfLifeAI* game = (GameOfLifeAI*)malloc(sizeof(GameOfLifeAI));
    game->map_size.x = width;
    game->map_size.y = height;
    game->speed = 20;
    game->time = 0;
    game->frequency = frequency;
    game->current = nb_team * nb_player_per_team;
    game->timeBeforeRegen = 20;
    game->players = (Player**)malloc(sizeof(Player*) * nb_team * nb_player_per_team);
    game->nb_team = nb_team;
    game->nb_player_per_team = nb_player_per_team;
    game->teams_name = teams_name;
    game->teams_slot = (int*)malloc(sizeof(int) * nb_team);
    for (int i = 0; i < nb_team; i++) {
        game->teams_slot[i] = nb_player_per_team;
    }

    printf("Creating players\n");
    game->players = init_players(game->players, nb_team, nb_player_per_team, width, height, teams_name);
    printf("Map creation\n");
    game->map = create_map(width, height);
    printf("Map created\n");
    generate_map(game->map, game->players, game->current);
    printf("Map generated\n");
    game->evolve_Iteration = 0;
    game->nearest = game->players[0];
    draw_map(game->map, "map.txt", game->players, game->current);
    return game;
}

// void reset_game(GameOfLifeAI* game) {
//     game->speed = time(NULL);
//     game->time = 0;
//     game->current = 0;
//     game->timeBeforeRegen = 20;
//     game->evolve_Iteration = 0;
//     game->nearest = game->player;
//     reset_player(game->player, create_point(20, 20), game->map_size, game->map_size, 1, 1);
//     reset_map(game->map, game->players);
// }

void nearest_player_same_level(GameOfLifeAI* game, Player* player) {
    Player* nearest = game->nearest;
    for (int i = 0; i < game->current; i++) {
        Player* play = game->players[i];
        if (play == player)
            continue;
        if (play->level == player->level) {
            if (nearest == game->players[0])
                nearest = play;
            else if (get_distance(player, nearest) > get_distance(player, play))
                nearest = play;
        }
    }
    game->nearest = nearest;
}

void check_evolution(GameOfLifeAI* game) {
    int reward = 0;
    bool addPlayer = false;
    Player** players_ = NULL;
    int players_count = 0;
    for (int i = 0; i < game->current; i++) {
        Player* player2 = game->players[i];
        int current = check_evolution_requirements(player2, game->players, game->current);
        if (current == 1) {
            printf("Player %d evolved\n", player2->id);
            addPlayer = true;
            players_ = (Player**)realloc(players_, (players_count + 1) * sizeof(Player*));
            players_[players_count] = player2;
            players_count++;
        }
    }
    for (int i = 0; i < players_count; i++) {
        Player* play = players_[i];
        reward += play->level;
        evolve(play);
    }
}

bool check_evolution_player(GameOfLifeAI* game, Player* player) {
    int current = check_evolution_requirements(player, game->players, game->current);
    if (current == 1) {
        evolve(player);
        return true;
    }
    return false;
}

void add_egg(GameOfLifeAI* game, Player* player) {
    game->evolve_Iteration = 50;
    Player* new_player = init_player(game->map_size.x, game->map_size.y, rand() % 1212121, 1, player->team);
    game->players = (Player**)realloc(game->players, (game->current + 2) * sizeof(Player*));
    new_player->appearance_time = game->time;
    // appearence time
    for (int i = 0; i < game->nb_team; i++) {
        if (strcmp(game->teams_name[i], player->team) == 0) {
            game->teams_slot[i]++;
            game->players = (Player**)realloc(game->players, (game->current + 2) * sizeof(Player*));
            game->players[game->current] = new_player;
            game->players[game->current + 1] = NULL; // Null terminator
            break;
        }
    }
    game->current++;
}

void add_player(GameOfLifeAI* game, Player* player) {
    game->players = (Player**)realloc(game->players, (game->current + 2) * sizeof(Player*));
    game->players[game->current] = player;
    game->players[game->current + 1] = NULL; // Null terminator
    game->current++;

    player->id = rand() % 100000;
}

Player* get_player(GameOfLifeAI* game, int index) {
    if (index >= 0 && index < game->current)
        return game->players[index];
    return NULL;
}

Ressource *get_resources(Map *map, Point position, map_size map_size) {
    // Get resources from the map
    int width = map_size.x;
    int height = map_size.y;
    Ressource *resources;
    if (position.x < width && position.y < height) {
        printf("map->map[position.x][position.y]: %d\n", map->map[position.x][position.y]);

        resources = &(map->map[position.x][position.y]);
    } else if (position.x < width) {
        resources = &(map->map[position.x][0]);
    } else if (position.y < height) {
        resources = &(map->map[0][position.y]);
    } else {
        resources = &(map->map[0][0]);
    }
    printf("Resources: %d\n", resources[0]);
    return resources;
}

void check_user_input(Player* player) {
    int rando = rand() % 4;
    printf("Rando: %d\n", rando);
    printf("Direction: %d\n", player->direction);
    int *action[4] = {0};
    action[rando] = 1;
    for (int i = 0; i < 4; i++) {
        printf("Action: %d\n", action[i]);
    }
    player->direction = choose_direction(action, player);
}

Player** get_players(GameOfLifeAI* game) {
    return game->players;
}

Ressource *check_next_move(GameOfLifeAI* game, Player* player, Point* player_pos) {
    Ressource *resources = NULL;
    game->time++;
    game->evolve_Iteration++;
    Point next_pos = get_position_after(player, player->direction);
    printf("Next pos: %d, %d\n", next_pos.x, next_pos.y);
    resources = game->map->map[next_pos.x][next_pos.y];
    for (int i = 0; resources[i] != 0; i++) {
        printf("Resources: %d\n", resources[i]);
    }
    *player_pos = next_pos;
    return resources;
}

bool verify_next_tile(GameOfLifeAI* game, Player* player, Point posNext, int* reward) {
    bool isPlayer = false;
    for (int i = 0; i < game->current; i++) {
        Player* play = game->players[i];
        if (play == player)
            continue;
        if (play->position.x == posNext.x && play->position.y == posNext.y) {
            isPlayer = true;
            break;
        }
    }
    if (isPlayer) {
        bool isPlayer2 = false;
        for (int i = 0; i < game->current; i++) {
            Player* playe = game->players[i];
            if (playe == player)
                continue;
            if (playe->position.x == player->position.x && playe->position.y == player->position.y) {
                *reward += 10;
                isPlayer2 = true;
                break;
            }
        }
        return false;
    }
    else {
        bool isPlayer2 = false;
        for (int i = 0; i < game->current; i++) {
            Player* playe = game->players[i];
            if (playe == player)
                continue;
            if (playe->position.x == player->position.x && playe->position.y == player->position.y) {
                *reward += 10;
                isPlayer2 = true;
                break;
            }
        }
        return true;
    }
}

int move_player(Player* player, int condition, Ressource* resources, Point player_pos) {
    int reward = 0;
    printf("Condition: %d\n", condition);
    if (condition) {
        printf("Moving player\n");
        player->position = player_pos;
        int length = sizeof(resources) / sizeof(resources[0]);
        printf("Length: %d\n", length);
        for (int i = 0; i < length; i++) {
            printf("Ressource: %d\n", resources[i]);
            Ressource resource = resources[i];
            if (resource == Ressource_FOOD) {
                player->timeBeforeDeath += 126;
                reward += 1;
            }
            if (resource != Ressource_EMPTY) {
                reward += 2;
                // add resource to player
                // check_resources(resources, player);
            }
            printf("Reward: %d\n", reward);
        }
    }
    else {
        player->position = player_pos;
    }
    return reward;
}


void check_game_state(GameOfLifeAI* game) {
    if (game->timeBeforeRegen <= 0) {
        printf("Regenerating ressources\n");
        game->timeBeforeRegen = 20;
        regenerate_ressources(game->map, game->players, game->current);
        printf("Ressources regenerated\n");
    }
}

int check_player_state(Player* player, int* reward) {
    int gameOver = false;
    if (player->timeBeforeDeath <= 0) {
        *reward -= 100;
        printf("Player %d died\n", player->id);
        gameOver = true;
    }
    return gameOver;
}

Direction choose_direction(int* action, Player* player) {
    Direction clock_wise[5] = { RIGHT, DOWN, LEFT, UP, NONE };
    int idx = 0;
    for (int i = 0; i < 5; i++) {
        if (player->direction == clock_wise[i]) {
            idx = i;
            break;
        }
    }

    if (action[0] == 1 && action[1] == 0 && action[2] == 0 && action[3] == 0) {
        player->direction = clock_wise[idx];
    }
    else if (action[0] == 0 && action[1] == 1 && action[2] == 0 && action[3] == 0) {
        player->direction = clock_wise[(idx + 1) % 5];
    }
    else if (action[0] == 0 && action[1] == 0 && action[2] == 1 && action[3] == 0) {
        int new_idx = idx - 1;
        if (new_idx < 0)
            new_idx = 4;
        player->direction = clock_wise[new_idx];
    }
    else {
        player->direction = clock_wise[4];
    }

    return player->direction;
}

Inventory check_resources(Ressource* resources, Player* player)
{
    int length = sizeof(resources) / sizeof(resources[0]);
    Inventory inventory = player->inventory;

    for (int i = 0; resources[i] != 0; i++) {
        Ressource resource = resources[i];
        switch (resource) {
            case Ressource_FOOD:
                inventory.food++;
                break;
            case Ressource_LIMEMATE:
                inventory.linemate++;
                break;
            case Ressource_DERAUMERE:
                inventory.deraumere++;
                break;
            case Ressource_SIBUR:
                inventory.sibur++;
                break;
            case Ressource_MENDIANE:
                inventory.mendiane++;
                printf("Mendiane: %d\n", inventory.mendiane);
                break;
            case Ressource_PHIRAS:
                inventory.phiras++;
                break;
            case Ressource_THYSTAME:
                inventory.thystame++;
                break;
            default:
                break;
        }
    }
    return inventory;
}