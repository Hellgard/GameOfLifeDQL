#include "../include/Game.h"
#include "../include/Player.h"
#include "../include/Map.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

GameOfLifeAI* create_game_of_life_ai(int width, int height) {
    GameOfLifeAI* game = (GameOfLifeAI*)malloc(sizeof(GameOfLifeAI));
    game->map_size.x = width;
    game->map_size.y = height;
    game->speed = 20;
    game->time = 0;
    game->current = 0;
    game->timeBeforeRegen = 20;
    printf("Creating player\n");
    int random = rand() % 1212121; // random number for the player
    game->player = init_player(width, height, random, 1);
    printf("Player created\n");
    game->players = (Player**)malloc(sizeof(Player*));
    printf("Creating map\n");
    game->players[0] = game->player;
    game->map = create_map(width, height);
    generate_map(game->map, game->players, game->current);
    game->evolve_Iteration = 0;
    game->nearest = game->player;
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
            if (nearest == game->player)
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

    // choose one of the players that can evolve
    if (addPlayer) {
        Player* player = (players_count > 0) ? players_[rand() % players_count] : game->player;
        game->evolve_Iteration = 50;
        Player* new_player = init_player(game->map_size.x, game->map_size.y, rand() % 1212121, 1);
        set_child(player, new_player);
        game->players = (Player**)realloc(game->players, (game->current + 2) * sizeof(Player*));
        new_player->appearance_time = game->time;
        game->players[game->current] = new_player;
        // appearence time
        game->players[game->current + 1] = NULL; // Null terminator
        game->current++;
        free(players_);
    }
}

void add_pending_players(GameOfLifeAI* game) {
    double current_time = game->time;
    for (int i = 0; i < game->current; i++) {
        Player* player2 = game->players[i];
        double appearance_time = player2->appearance_time;
        Player** players = NULL;
        int players_count = 0;
        for (int j = 0; j < game->current; j++) {
            Player* player1 = game->players[j];
            if (player1 != player2) {
                players = (Player**)realloc(players, (players_count + 1) * sizeof(Player*));
                players[players_count] = player1;
                players_count++;
            }
        }
        if (appearance_time + 2 < current_time) {
            game->players = (Player**)realloc(game->players, (game->current + 1) * sizeof(Player*));
            game->players[game->current] = player2;
            game->current++;
            for (int j = 0; j < players_count; j++)
                set_child(players[j], player2);
            free(players);
        }
    }
}

void add_player(GameOfLifeAI* game, Player* player) {
    game->players = (Player**)realloc(game->players, (game->current + 1) * sizeof(Player*));
    game->players[game->current] = player;
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
        resources = &(map->map[position.x][position.y]);
    } else if (position.x < width) {
        resources = &(map->map[position.x][0]);
    } else if (position.y < height) {
        resources = &(map->map[0][position.y]);
    } else {
        resources = &(map->map[0][0]);
    }
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
    resources = get_resources(game->map, next_pos, game->map_size);
    printf("Resources: %d\n", resources[0]);
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
        if (!isPlayer2)
            update_map(game->map, player->position, Ressource_EMPTY);
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
        if (!isPlayer2)
            update_map(game->map, player->position, Ressource_EMPTY);
        return true;
    }
}

int move_player(Player* player, int condition, Ressource* resources) {
    int reward = 0;
    printf("Condition: %d\n", condition);
    if (condition) {
        printf("Moving player\n");
        player->position = move(player);
        printf("Player position: %d, %d\n", player->position.x, player->position.y);
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
                check_resources(resources, player);
            }
            printf("Reward: %d\n", reward);
        }
    }
    else {
        player->position = move(player);
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
    Direction clock_wise[5] = { RIGHT, LEFT, UP, DOWN, NONE };
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

void check_resources(Ressource* resources, Player* player)
{
    int length = sizeof(resources) / sizeof(resources[0]);

    for (int i = 0; i < length; i++) {
        Ressource resource = resources[i];
        if (resource != Ressource_EMPTY) {
            switch (resource) {
                case Ressource_FOOD:
                    player->inventory.food++;
                    break;
                case Ressource_LIMEMATE:
                    player->inventory.linemate++;
                    break;
                case Ressource_DERAUMERE:
                    player->inventory.deraumere++;
                    break;
                case Ressource_SIBUR:
                    player->inventory.sibur++;
                    break;
                case Ressource_MENDIANE:
                    player->inventory.mendiane++;
                    break;
                case Ressource_PHIRAS:
                    player->inventory.phiras++;
                    break;
                case Ressource_THYSTAME:
                    player->inventory.thystame++;
                    break;
                default:
                    break;
            }
        }
    }
}