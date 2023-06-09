/*
** EPITECH PROJECT, 2021
** Player.c
** File description:
** Player
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../include/Player.h"
#include "../include/Enum.h"
#include "../include/Constants.h"

Player *init_player(int width, int height, int random, int level, char *team) {
    int levelRequirements[7][7] = {
        {1, 1, 0, 0, 0, 0, 0},
        {2, 1, 1, 1, 0, 0, 0},
        {2, 2, 0, 1, 0, 2, 0},
        {4, 1, 1, 2, 0, 1, 0},
        {4, 1, 2, 1, 3, 0, 0},
        {6, 1, 2, 3, 0, 1, 0},
        {6, 2, 2, 2, 2, 2, 1}
    };
    Player* player;
    player = malloc(sizeof(Player));
    player->width = width;
    player->height = height;
    int x = rand() % width;
    int y = rand() % height;
    player->position = (Point){x, y};
    player->direction = RIGHT;
    player->team = strdup(team);
    init_inventory(&(player->inventory));
    player->level = level;
    player->id = random;
    player->timeBeforeDeath = 3260 / 100;
    player->is_egg = 1;
    player->is_dead = 0;

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            player->levelRequirements[i][j] = levelRequirements[i][j];
        }
    }
    return player;
}

void play_sound(const char* sound_file) {
    // Play sound implementation
    FILE* file = fopen(sound_file, "r");
    if (file == NULL) {
        printf("Error: can't open file %s\n", sound_file);
        return;
    }
    fclose(file);
}

int get_distance(Player* player, Player* other_player) {
    return abs(player->position.x - other_player->position.x) + abs(player->position.y - other_player->position.y);
}


int check_evolution_requirements(Player* player, Player** players, int num_players) {
    if (player->level == 8) {
        return 0;
    }

    int number_of_players = 1;
    if (player->level > 1) {
        for (int i = 0; i < num_players; i++) {
            if (player->id != players[i]->id) {
                if (player->position.x == players[i]->position.x && player->position.y == players[i]->position.y) {
                    if (players[i]->level >= player->levelRequirements[player->level - 1][0]) {
                        number_of_players++;
                    }
                }
            }
        }
    }

    if (number_of_players < player->playerRequirements[player->level - 1]) {
        return 0;
    }

    if (can_evolve(player) == 0) {
        return 0;
    }

    Ressource resources[6] = {Ressource_LIMEMATE, Ressource_DERAUMERE, Ressource_SIBUR, Ressource_MENDIANE, Ressource_PHIRAS, Ressource_THYSTAME};
    int numbers[6];
    for (int i = 0; i < 6; i++) {
        numbers[i] = player->levelRequirements[player->level - 1][i + 1];
    }

    // play_sound("data/vorbis.mp3");
    remove_resources(&(player->inventory), resources, numbers, 6);

    return 1;
}

int can_evolve(Player* player) {
    if (player->level == 8) {
        return 0;
    }

    for (int i = 1; i < 7; i++) {
        switch (i) {
            case 1:
                if (player->inventory.linemate < player->levelRequirements[player->level - 1][i]) {
                    return 0;
                }
                break;
            case 2:
                if (player->inventory.deraumere < player->levelRequirements[player->level - 1][i]) {
                    return 0;
                }
                break;
            case 3:
                if (player->inventory.sibur < player->levelRequirements[player->level - 1][i]) {
                    return 0;
                }
                break;
            case 4:
                if (player->inventory.mendiane < player->levelRequirements[player->level - 1][i]) {
                    return 0;
                }
                break;
            case 5:
                if (player->inventory.phiras < player->levelRequirements[player->level - 1][i]) {
                    return 0;
                }
                break;
            case 6:
                if (player->inventory.thystame < player->levelRequirements[player->level - 1][i]) {
                    return 0;
                }
                break;
        }
    }

    return 1;
}

int get_id(Player* player) {
    return player->id;
}

Point move(Player* player) {
    player->timeBeforeDeath--;

    if (player->direction == NONE) {
        return;
    }

    if (player->direction == UP) {
        player->position.y--;
    } else if (player->direction == DOWN) {
        player->position.y++;
    } else if (player->direction == LEFT) {
        player->position.x--;
    } else if (player->direction == RIGHT) {
        player->position.x++;
    }

    if (player->position.x < 0) {
        player->position.x = player->width - 1;
    } else if (player->position.x >= player->width) {
        player->position.x = 0;
    } else if (player->position.y < 0) {
        player->position.y = player->height - 1;
    } else if (player->position.y >= player->height) {
        player->position.y = 0;
    }
    return player->position;
}

Inventory* get_inventory(Player* player) {
    return &(player->inventory);
}

int get_level(Player* player) {
    return player->level;
}

Direction get_direction(Player* player) {
    return player->direction;
}

Point get_position(Player* player) {
    return player->position;
}

Point get_position_after(Player* player, Direction direction) {
    Point position = player->position;

    if (direction == UP) {
        position.y--;
    } else if (direction == DOWN) {
        position.y++;
    } else if (direction == LEFT) {
        position.x--;
    } else if (direction == RIGHT) {
        position.x++;
    }

    if (position.x < 0) {
        position.x = player->width - 1;
    } else if (position.x >= player->width) {
        position.x = 0;
    } else if (position.y < 0) {
        position.y = player->height - 1;
    } else if (position.y >= player->height) {
        position.y = 0;
    }

    return position;
}

void set_direction(Player* player, Direction direction) {
    player->direction = direction;
}

void set_position(Player* player, Point position) {
    player->position = position;
}

void set_level(Player* player, int level) {
    player->level = level;
}

void evolve(Player* player) {
    player->level++;
}


void show_player(Player* player) {
    printf("------------------\n");
    printf("Player\n");
    printf("id: %d\n", player->id);
    printf("level: %d\n", player->level);
    printf("position: %d, %d\n", player->position.x, player->position.y);
    printf("direction: %d\n", player->direction);
    printf("inventory: Food = %d, Linemate = %d, Deraumere = %d, Sibur = %d, Mendiane = %d, Phiras = %d, Thystame = %d\n", player->inventory.food, player->inventory.linemate, player->inventory.deraumere, player->inventory.sibur, player->inventory.mendiane, player->inventory.phiras, player->inventory.thystame);
    printf("------------------\n");
}