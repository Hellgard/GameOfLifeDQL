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
    while (clock() < start_time + time);
}

int main(void)
{
    srand(time(NULL));

    GameOfLifeAI *game;
    int width = 60;
    int height = 60;
    // Reset the game
    game = create_game_of_life_ai(width, height);

    // Game loop
    // int action[5];
    Ressource *ressources = malloc(sizeof(Ressource) * 7);
    Point player_pos;
    int reward;
    while (1) {
        check_user_input(game->player);
        check_next_move(game, game->player, &ressources, &player_pos);
        int condition = verify_next_tile(game, game->player, player_pos, &reward);
        move_player(game->player, condition, ressources);
        nearest_player_same_level(game, game->player);
        check_player_state(game->player, &reward);
        game->timeBeforeRegen--;
        check_game_state(game);
        draw_map(game->map, "map.png");
        check_evolution(game);
        add_pending_players(game);
        wait_time(game->speed);
    }

    return 0;
}