/*
** EPITECH PROJECT, 2021
** main.c
** File description:
** main
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "include/Game.h"
#include <stdio.h>

void wait_time(int time)
{
    clock_t start_time = clock();
    while (clock() < start_time + 200000);
}

int main(void)
{
    srand(time(NULL));

    GameOfLifeAI *game;
    int width = 10;
    int height = 10;
    // Reset the game
    printf("Creating game\n");
    game = create_game_of_life_ai(width, height);
    printf("Game created\n");

    // Game loop
    // int action[5];
    Ressource *ressources = malloc(sizeof(Ressource) * 7);
    Point player_pos;
    int reward;
    printf("Starting game loop\n");
    while (1) {
        check_user_input(game->player);
        ressources = check_next_move(game, game->player, &player_pos);
        int condition = verify_next_tile(game, game->player, player_pos, &reward);
        printf("starting move player\n");
        printf("ressources[0] = %d\n", ressources[0]);

        move_player(game->player, condition, ressources);
        printf("starting check player nearest\n");
        nearest_player_same_level(game, game->player);
        printf("starting check player state\n");
        check_player_state(game->player, &reward);
        printf("starting check game state\n");
        game->timeBeforeRegen--;
        printf("time before regen = %d\n", game->timeBeforeRegen);
        check_game_state(game);
        printf("starting draw map\n");

        draw_map(game->map, "map.txt", game->players, game->current);
        
        printf("starting check evolution\n");
        check_evolution(game);
        printf("starting add pending players\n");
        add_pending_players(game);
        printf("starting wait time\n");
        wait_time(game->speed);
    }

    return 0;
}