/*
** EPITECH PROJECT, 2021
** command.c
** File description:
** command
*/

#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void write_to_file(char *filename, char *str)
{
    printf("Writing to file:\n");
    FILE *fp = fopen(filename, "a"); // open file in append mode
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fp, "\n%s", str); // write string to file with a space separator
    fprintf(fp, "\n"); // end with newline character
    fclose(fp); // close file
}

void create_file(char* string) {
    FILE *file;
    char *filename = malloc(strlen(string) + 5); // Add space for ".txt" extension
    if (!filename) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    sprintf(filename, "%s.txt", string); // Add ".txt" extension to filename
    
    file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Unable to create file %s\n", filename);
        free(filename);
        return;
    }
    
    fprintf(file, "This is a test file for %s\n", string);
    
    fclose(file);
    free(filename);
}

void file_setup(char *user, char *user2, char *filename, char *filename2)
{
    strcat(filename, user);
    strcat(filename, "|");
    strcat(filename, user2);
    strcat(filename, ".txt");
    strcat(filename2, user2);
    strcat(filename2, "|");
    strcat(filename2, user);
    strcat(filename2, ".txt");
}

char *search_file(char *user, char *user2)
{
    char *flname = malloc(sizeof(char) * (strlen(user) + strlen(user2) + 2));
    char *flname2 = malloc(sizeof(char) * (strlen(user) + strlen(user2) + 2));
    file_setup(user, user2, flname, flname2);

    FILE *fp;
    fp = fopen(flname, "r");
    if (fp == NULL) {
        fp = fopen(flname2, "r");
        if (fp == NULL) {
            return (NULL);
        } else {
            return (flname2);
        }
    } else {
        return (flname);
    }
}

void do_conv(Server_t *server, char *message, char *user)
{
    char *tmp = malloc(sizeof(char) * (strlen(message) + strlen(user) + 3));
    char *tmp2 = malloc(sizeof(char) *
    (strlen(server->clients[server->index].username) + strlen(user) + 3));

    strcpy(tmp, server->clients[server->index].username);
    strcat(tmp, ": ");
    strcat(tmp, message);
    strcpy(tmp2, server->clients[server->index].username);
    strcat(tmp2, "|");
    strcat(tmp2, user);
    if (search_file(server->clients[server->index].username, user) == NULL) {
        create_file(tmp2);
        strcat(tmp2, ".txt");
    } else {
        tmp2 = search_file(server->clients[server->index].username, user);
    }
    write_to_file(tmp2, tmp);
}

int read_user_file(char *filename, char usernames[MAX_CLIENTS][20],
char passwords[MAX_CLIENTS][20]) {
    FILE *fp;
    char line[50];
    int i = 0;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open file %s\n", filename);
        return 0;
    }
    while (fgets(line, 50, fp) != NULL && i < MAX_CLIENTS) {
        char *username = strtok(line, " ");
        char *password = strtok(NULL, "\n");
        if (username == NULL || password == NULL) {
            continue;
        }
        strcpy(usernames[i], username);
        strcpy(passwords[i], password);
        i++;
    }
    fclose(fp);
    return i;
}

char *remove_first_word(char* sentence)
{
    char* remaining_sentence = sentence;

    while (*remaining_sentence != ' ' && *remaining_sentence != '\0')
        remaining_sentence++;
    if (*remaining_sentence == ' ')
        remaining_sentence++;    
    return remaining_sentence;
}

int help(Server_t *server)
{
    char *help = "214-The following commands are recognized.\n";
    char *help2 = "/Login (Username) /password (password) /logout";
    char *help2_5 = "/users /send (user) (message)\n";
    char *help3 = "/help, /quit.\n";
    int help_len = strlen(help) + strlen(help2) + strlen(help2_5) +
    strlen(help3) + 1;
    char *help_global = malloc(sizeof(char) * help_len);

    strcpy(help_global, help);
    strcat(help_global, help2);
    strcat(help_global, help3);
    write(server->clients[server->index].fd, help_global, strlen(help_global));
    return (0);
}

int username_error_handle(bool found, char *user, Server_t *server)
{
    if (found) {
        write(server->clients[server->index].fd, "Username already taken.\n"
        , 24);
        return (0);
    } else if (user == NULL) {
        write(server->clients[server->index].fd, "Wrong username.\n", 16);
        return (0);
    } else if (strlen(user) > 32) {
        write(server->clients[server->index].fd, "Username too long.\n", 19);
        return (0);
    }
    return (1);
}

int password_error_handle2(bool found, char *password, Server_t *server)
{
    char usernames[MAX_CLIENTS][20];
    char passwords[MAX_CLIENTS][20];
    int nb_users = read_user_file("../pass.txt", usernames, passwords);

    if (server->clients[server->index].logged == 1) {
        for (int i = 0; i < nb_users; i++) {
            if (strcmp(passwords[i], password) == 0 && strcmp(usernames[i],
            server->clients[server->index].username) == 0) {
                found = true;
                break;
            }
        }
        if (!found) {
            write(server->clients[server->index].fd, "Wrong password.\n", 16);
            return (0);
        }
    } else if (server->clients[server->index].logged == 3) {
        register_pass_to_file(password, server);
    }
    return (1);
}

void register_pass_to_file(char *password, Server_t *server)
{
        char *user_pass = malloc(sizeof(char) *(strlen(server->clients
        [server->index].username) + strlen(password) + 2));
        strcpy(user_pass, server->clients[server->index].username);
        strcat(user_pass, " ");
        strcat(user_pass, password);
        write_to_file("../pass.txt", user_pass);
        free(user_pass);
}

int password_error_handle(bool found, char *password, Server_t *server)
{
    if (found) {
        write(server->clients[server->index].fd, "Password already taken.\n"
        , 24);
        return (0);
    } else if (password == NULL) {
        write(server->clients[server->index].fd, "Wrong password.\n", 16);
        return (0);
    } else if (strlen(password) > 32) {
        write(server->clients[server->index].fd, "Password too long.\n", 19);
        return (0);
    }
    return (1);
}

int password(Server_t *server, int fd, GameOfLifeAI *game)
{
    char *token = strtok(server->buffer, " ");
    char *password = strtok(NULL, "\r ");
    char *register_password = "230 User logged in.\n";
    char usernames[MAX_CLIENTS][20];
    char passwords[MAX_CLIENTS][20];
    int nb_users = read_user_file("../pass.txt", usernames, passwords);
    bool found = false;

    if (password_error_handle(found, password, server) == 0)
        return (0);
    if (password_error_handle2(found, password, server) == 0)
        return (0);
    server->clients[server->index].logged = 2;
    server->clients[server->index].password = strdup(password);
    write(server->clients[server->index].fd, register_password,
    strlen(register_password));
    return (0);
}

int login_error_handle(bool found, char *user, Server_t *server)
{
    if (user == NULL) {
        write(server->clients[server->index].fd, "Wrong username.\n", 16);
        return (0);
    } else if (!found) {
        server->clients[server->index].logged = 1;
        write(server->clients[server->index].fd, "User not found.\n", 16);
        return (0);
    } else if (found) {
        server->clients[server->index].logged = 1;
        server->clients[server->index].username = strdup(user);
        write(server->clients[server->index].fd,
        "User found. proceed with password.\n", 35);
        return (0);
    }
    return (1);
}

int login(Server_t *server, int fd, GameOfLifeAI *game)
{   
    char *token = strtok(server->buffer, " ");
    char *user = strtok(NULL, "\r ");
    if (user == NULL) {
        write(server->clients[server->index].fd, "Wrong username.\n", 16);
        return (0);
    }
    char *register_user = "230 Username correct, proceed with pass.\n";
    char usernames[MAX_CLIENTS][20];
    char passwords[MAX_CLIENTS][20];
    int nb_users = read_user_file("../pass.txt", usernames, passwords);
    bool found = false;

    if (server->clients[server->index].logged != 0) {
        write(server->clients[server->index].fd,
        "You are already logged in.\n", 27);
        return (0);
    }
    for (int i = 0; i < nb_users; i++) {
        if (strcmp(usernames[i], user) == 0) {
            found = true;
            break;
        }
    }
    if (login_error_handle(found, user, server) == 0)
        return (0);
    return (0);
}

int logout(Server_t *server, int fd, GameOfLifeAI *game)
{
    if (server->clients[server->index].logged == 2) {
        server->clients[server->index].logged = 0;
        write(server->clients[server->index].fd, "221 Goodbye.\n", 13);
        return (0);
    } else {
        write(server->clients[server->index].fd, "530 Not logged in.\n", 19);
        return (0);
    }
}

int noop(Server_t *server, int fd, GameOfLifeAI *game)
{
    char *noop = "200 Command okay.\n";

    write(server->clients[server->index].fd, noop, strlen(noop));
    return (0);
}

int users(Server_t *server, int fd, GameOfLifeAI *game)
{
    if (server->clients[server->index].logged == 2) {
        char usernames[MAX_CLIENTS][20];
        char passwords[MAX_CLIENTS][20];
        int nb_users = read_user_file("../pass.txt", usernames, passwords);
        char *users = malloc(sizeof(char) * (nb_users * 20 + 1));
        char *tmp = malloc(sizeof(char) * 21);

        strcpy(users, "");
        for (int i = 0; i < nb_users; i++) {
            strcpy(tmp, usernames[i]);
            strcat(tmp, " ");
            strcat(users, tmp);
        }
        strcat(users, "\n");
        write(server->clients[server->index].fd, users, strlen(users));
        free(users);
        free(tmp);
        return (0);
    } else {
        write(server->clients[server->index].fd, "530 Not logged in.\n", 19);
        return (0);
    }
}

int user_error_handle(bool found, char *user, Server_t *server)
{
    if (user == NULL) {
        write(server->clients[server->index].fd, "Wrong username.\n", 16);
        return (0);
    } else if (!found) {
        write(server->clients[server->index].fd, "User not found.\n", 16);
        return (0);
    } else if (found) {
        write(server->clients[server->index].fd, "User found.\n", 12);
        return (0);
    }
    return (1);
}


int user(Server_t *server, int fd, GameOfLifeAI *game)
{
    if (server->clients[server->index].logged == 2) {
        char *token = strtok(server->buffer, " ");
        char *user = strtok(NULL, "\r ");
        char usernames[MAX_CLIENTS][20];
        char passwords[MAX_CLIENTS][20];
        int nb_users = read_user_file("../pass.txt", usernames, passwords);
        bool found = false;

        for (int i = 0; i < nb_users; i++) {
            if (strcmp(usernames[i], user) == 0) {
                found = true;
                break;
            }
        }
        if (user_error_handle(found, user, server) == 0)
            return (0);
    } else {
        write(server->clients[server->index].fd, "530 Not logged in.\n", 19);
        return (0);
    }
    return (0);
}

void send_message(Server_t *server, char *message, char *user)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->clients[i].logged == 2 &&
        strcmp(server->clients[i].username, user) == 0) {
            char *message_to_send = malloc(sizeof(char) *
            (strlen(message) + 20));
            char *tmp = malloc(sizeof(char) * (strlen(message) + 20));
            do_conv(server, message, user);
            strcpy(tmp, server->clients[server->index].username);
            strcat(tmp, ": ");
            strcat(tmp, message);
            strcpy(message_to_send, tmp);
            write(server->clients[i].fd, message_to_send,
            strlen(message_to_send));
            free(message_to_send);
            free(tmp);
            return (0);
        }
    }
}

int sent_to_error_handle(bool found, char *user, Server_t *server,
char *message)
{
    if (user == NULL) {
        write(server->clients[server->index].fd, "Wrong username.\n", 16);
        return (0);
    } else if (!found) {
        write(server->clients[server->index].fd, "User not found.\n", 16);
        return (0);
    } else if (found) {
        write(server->clients[server->index].fd, "User found.\n", 12);
        send_message(server, message, user);
    }
    return (1);
}

int send_to(Server_t *server, int fd, GameOfLifeAI *game)
{
    char *message1 = strdup(remove_first_word(server->buffer));
    char *message = strdup(remove_first_word(message1));
    char *token = strtok(server->buffer, " ");
    char *user = strtok(NULL, " ");

    char usernames[MAX_CLIENTS][20];
    char passwords[MAX_CLIENTS][20];
    int nb_users = read_user_file("../pass.txt", usernames, passwords);
    bool found = false;

  
    if (server->clients[server->index].logged == 2) {
        for (int i = 0; i < nb_users; i++) {
            if (strcmp(usernames[i], user) == 0) {
                found = true;
                printf("found user %s\n", user);
                break;
            }
        }
        if (sent_to_error_handle(found, user, server, message) == 0)
            return (0);
        } else {
            write(server->clients[server->index].fd, "530 Not logged in.\n",
            19);
            return (0);
    }
    return (0);
}

int messages(Server_t *server, int fd, GameOfLifeAI *game)
{
    char *messages = "530 Not logged in.\n";

    write(server->clients[server->index].fd, messages, strlen(messages));
    return (0);
}

int quit(Server_t *server, int fd, GameOfLifeAI *game)
{
    char *quit = "221 Service closing control connection.\n";

    write(server->clients[server->index].fd, quit, strlen(quit));
    close(server->clients[server->index].fd);
    return (0);
}

int forward(Server_t *server, int fd, GameOfLifeAI *game)
{
    char *forward = "forward\n";

    write(server->clients[server->index].fd, forward, strlen(forward));
    return (0);
}

int look(Server_t *server, int fd, GameOfLifeAI *game)
{
    printf("look\n");
    Ressource *ressources;
    int players_nb = game->current;
    for (int i = 0; i < players_nb; i++) {
        if (game->players[i]->id == server->clients[server->index].player_id) {
            int pos_x = game->players[i]->position.x;
            int pos_y = game->players[i]->position.y;
            printf("direction = %d\n", game->players[i]->direction);
            Point pos = get_position_after(game->players[i] , game->players[i]->direction);
            printf("pos_x = %d\n", pos.x);
            printf("pos_y = %d\n", pos.y);
            ressources = game->map->map[pos.y][pos.x];
            break;
        }
    }
    for (int i = 0; ressources[i] != 0; i++) {
        printf("%d\n", ressources[i]);
    }
    int len = 0;
    if (ressources != NULL) {
        printf("ressources not null\n");
        for (int i = 0; ressources[i] != 0; i++) {
            if (ressources[i] == Ressource_FOOD) {
                printf("food\n");
                len += strlen("food");
            } else if (ressources[i] == Ressource_LIMEMATE) {
                printf("linemate\n");
                len += strlen("linemate");
            } else if (ressources[i] == Ressource_DERAUMERE) {
                printf("deraumere\n");
                len += strlen("deraumere");
            } else if (ressources[i] == Ressource_SIBUR) {
                printf("sibur\n");
                len += strlen("sibur");
            } else if (ressources[i] == Ressource_MENDIANE) {
                printf("mendiane\n");
                len += strlen("mendiane");
            } else if (ressources[i] == Ressource_PHIRAS) {
                printf("phiras\n");
                len += strlen("phiras");
            } else if (ressources[i] == Ressource_THYSTAME) {
                printf("thystame\n");
                len += strlen("thystame");
            } else if (ressources[i] == Ressource_PLAYER) {
                printf("player\n");
                len += strlen("player");
            } else {
                printf("Empty\n");
                len += strlen("Empty");
            }
            len += 1;
        }
    }
    char *ressource = malloc(sizeof(char) * len + 1);
    ressource[0] = '\0';  // Initialize the string with null terminator

    for (int i = 0; ressources[i] != 0; i++) {
        if (ressources[i] == Ressource_FOOD) {
            strcat(ressource, "food\n");
        } else if (ressources[i] == Ressource_LIMEMATE) {
            strcat(ressource, "linemate\n");
        } else if (ressources[i] == Ressource_DERAUMERE) {
            strcat(ressource, "deraumere\n");
        } else if (ressources[i] == Ressource_SIBUR) {
            strcat(ressource, "sibur\n");
        } else if (ressources[i] == Ressource_MENDIANE) {
            strcat(ressource, "mendiane\n");
        } else if (ressources[i] == Ressource_PHIRAS) {
            strcat(ressource, "phiras\n");
        } else if (ressources[i] == Ressource_THYSTAME) {
            strcat(ressource, "thystame\n");
        } else if (ressources[i] == Ressource_PLAYER) {
            strcat(ressource, "player\n");
        } else {
            strcat(ressource, "Empty\n");
        }
    }
    printf("ressources: %s\n", ressource);
    write(server->clients[server->index].fd, ressource, strlen(ressource));

    return (0);
}

int int_len(int nb)
{
    int len = 0;

    while (nb > 0) {
        nb /= 10;
        len++;
    }
    return (len);
}

char *int_to_str(int nb)
{
    int len = int_len(nb);
    char *str = malloc(sizeof(char) * len + 1);

    if (nb == 0)
        return ("0");
    str[len] = '\0';
    for (int i = len - 1; i >= 0; i--) {
        str[i] = nb % 10 + '0';
        nb /= 10;
    }
    printf("str: %s\n", str);
    return (str);
}

int inventory(Server_t *server, int fd, GameOfLifeAI *game)
{
    printf("inventory\n");
    
    int food = game->players[server->clients[server->index].player_index]->inventory.food;
    int linemate = game->players[server->clients[server->index].player_index]->inventory.linemate;
    int deraumere = game->players[server->clients[server->index].player_index]->inventory.deraumere;
    int sibur = game->players[server->clients[server->index].player_index]->inventory.sibur;
    int mendiane = game->players[server->clients[server->index].player_index]->inventory.mendiane;
    int phiras = game->players[server->clients[server->index].player_index]->inventory.phiras;
    int thystame = game->players[server->clients[server->index].player_index]->inventory.thystame;

    int food_len = int_len(food) + 5;// f o o d : -> 5
    int linemate_len = int_len(linemate) + 9;// l i n e m a t e : -> 9
    int deraumere_len = int_len(deraumere) + 10;// d e r a u m e r e : -> 10
    int sibur_len = int_len(sibur) + 6;// s i b u r : -> 6
    int mendiane_len = int_len(mendiane) + 9;// m e n d i a n e : -> 8
    int phiras_len = int_len(phiras) + 7;// p h i r a s : -> 7
    int thystame_len = int_len(thystame) + 9;// t h y s t a m e : -> 9

    int len = food_len + linemate_len + deraumere_len + sibur_len + mendiane_len + phiras_len + thystame_len + 7;
    char *ressource = malloc(sizeof(char) * len + 1);
    ressource[0] = '\0';  // Initialize the string with null terminator
    strcat(ressource, "food: ");
    strcat(ressource, int_to_str(food));
    strcat(ressource, "\n");
    strcat(ressource, "linemate: ");
    strcat(ressource, int_to_str(linemate));
    strcat(ressource, "\n");
    strcat(ressource, "deraumere: ");
    strcat(ressource, int_to_str(deraumere));
    strcat(ressource, "\n");
    strcat(ressource, "sibur: ");
    strcat(ressource, int_to_str(sibur));
    strcat(ressource, "\n");
    strcat(ressource, "mendiane: ");
    strcat(ressource, int_to_str(mendiane));
    strcat(ressource, "\n");
    strcat(ressource, "phiras: ");
    strcat(ressource, int_to_str(phiras));
    strcat(ressource, "\n");
    strcat(ressource, "thystame: ");
    strcat(ressource, int_to_str(thystame));
    strcat(ressource, "\n");

    write(server->clients[server->index].fd, ressource, strlen(ressource));

    return (0);
}

void process_broadcast(Server_t *server, GameOfLifeAI *game, char *message) {
    if (strstr(message, "canEvolve") != NULL) {
        Player *currentPlayer = game->players[server->clients[server->index].player_index];
        int canEvolve = check_evolution_requirements(currentPlayer, game->players, game->current);
        char response[6];  // "true\0" or "false\0"team_max_childs
        sprintf(response, "%s", canEvolve ? "true" : "false");
        write(server->clients[server->index].fd, response, strlen(response));
    } else if (strstr(message, "nearestPlayer") != NULL) {
        Player *currentPlayer = game->players[server->clients[server->index].player_index];
        nearest_player_same_level(game, currentPlayer);
        Player *nearestPlayer = game->nearest;

        // Compose the response string with nearest player's information
        char response[256];
        snprintf(response, sizeof(response), "Nearest player: ID=%d, X=%d, Y=%d\n",
                 nearestPlayer->id, nearestPlayer->position.x, nearestPlayer->position.y);
        write(server->clients[server->index].fd, response, strlen(response));
    } else
    {
        write(server->clients[server->index].fd, "Command not compatible with broadcast\ncanEvolve and nearestPlayer are the only ones supported\n", 100);
    }
    
}

int broadcast(Server_t *server, int fd, GameOfLifeAI *game)
{
    printf("broadcast\n");
    process_broadcast(server, game, server->buffer);
    return (0);
}

void process_connect_nbr(Server_t *server, GameOfLifeAI *game) {
    int total_length = 0;
    char *teams_slot = NULL;

    for (int i = 0; i < game->nb_team; i++) {
        int team_slot = 0;
        int len_team_slot = 1;

        // Calculate the total players of the same team
        for (int j = 0; j < game->current; j++) {
            if (strcmp(game->players[j]->team, game->teams_name[i]) == 0) {
                team_slot++;
               
            }
        }
         if (team_slot % 10 == 0) {
            len_team_slot++;
        }

        // Calculate the total length of the concatenated strings
        total_length += strlen(int_to_str(team_slot)) + 9; // Length of " slots\n"

        // Add the current team information to teams_slot
        if (i == 0) {
            teams_slot = malloc(total_length + 1); // +1 for the null terminator
            sprintf(teams_slot, "%s %d slots\n", game->players[i]->team, team_slot);
        } else {
            teams_slot = realloc(teams_slot, total_length + 1); // +1 for the null terminator
            sprintf(teams_slot, "%s%s %d slots\n", teams_slot, game->players[i]->team, team_slot);
        }
    }

    // Send the teams_slot information back to the client
    write(server->clients[server->index].fd, teams_slot, strlen(teams_slot));

    // Free the dynamically allocated memory
    free(teams_slot);
}

void process_connect_nbr_team(Server_t *server, GameOfLifeAI *game) {
    char* team_name = game->players[server->index]->team;
    int team_max_childs = 0; // Total max_childs for the player's team
    int team_slot = 0; // Total players of the same team
    int len_team_slot = 1;
    int i = 0;
    for (; i < game->nb_team; i++) {
        if (strcmp(game->teams_name[i], team_name) == 0) {
            break;
        }
    }
    team_slot = game->teams_slot[i];
    if (team_slot % 10 == 0) {
        len_team_slot++;
    }
    int total_length = len_team_slot + 9; // Length of " slots\n"
    char* teams_slot = malloc(total_length + 1); // +1 for the null terminator
    sprintf(teams_slot, "%s %d slots\n", team_name, team_slot);

    // Send the teams_slot information back to the client
    write(server->clients[server->index].fd, teams_slot, strlen(teams_slot));

    // Free the dynamically allocated memory
    free(teams_slot);
}

int connect_nbr(Server_t *server, int fd, GameOfLifeAI *game)
{
    printf("connect_nbr\n");
    // process_connect_nbr(server, game); // all slots for all teams
    process_connect_nbr_team(server, game); // all slots for the player's team
    return (0);
}

int ressource_to_str(Ressource resource) {
    switch (resource) {
        case Ressource_FOOD:
            return "food";
        case Ressource_LIMEMATE:
            return "linemate";
        case Ressource_DERAUMERE:
            return "deraumere";
        case Ressource_SIBUR:
            return "sibur";
        case Ressource_MENDIANE:
            return "mendiane";
        case Ressource_PHIRAS:
            return "phiras";
        case Ressource_THYSTAME:
            return "thystame";

    }
}

char *resources_to_string(Ressource *resources) {
    char *string = malloc(256);
    int length = 0;

    for (int i = 0; resources[i] != 0; i++) {
        char *resource_name = ressource_to_str(resources[i]);
        length += strlen(resource_name) + 1; // +1 for the space
        string = realloc(string, length + 1); // +1 for the null terminator
        strcat(string, resource_name);
        strcat(string, " ");
    }

    return string;
}

void process_take(Server_t* server, GameOfLifeAI* game) {
    printf("take\n");
    int player_index = server->clients[server->index].player_index;
    int player_x = game->players[player_index]->position.x;
    int player_y = game->players[player_index]->position.y;
    printf("player %d is at (%d, %d)\n", player_index, player_x, player_y);
    // Retrieve the resources on the current tile
    Ressource* resources = game->map->map[player_y][player_x];
    Inventory* player_inventory = &(game->players[player_index]->inventory);
    for (int i = 0; resources[i] != 0; i++) {
        printf("%d\n", resources[i]);
    }
    // Loop through the resources and add them to the player's inventory
    game->players[player_index]->inventory = check_resources(resources, game->players[player_index]);
    printf("player %d inventory.food = %d\n", player_index, player_inventory->food);
    printf("player %d inventory.linemate = %d\n", player_index, player_inventory->linemate);
    printf("player %d inventory.deraumere = %d\n", player_index, player_inventory->deraumere);
    printf("player %d inventory.sibur = %d\n", player_index, player_inventory->sibur);
    printf("player %d inventory.mendiane = %d\n", player_index, player_inventory->mendiane);
    printf("player %d inventory.phiras = %d\n", player_index, player_inventory->phiras);
    printf("player %d inventory.thystame = %d\n", player_index, player_inventory->thystame);

    Ressource *ressource = malloc(sizeof(Ressource) * 2);
    ressource[0] = Ressource_EMPTY;
    ressource[1] = 0;
    Point point;
    point.x = player_x;
    point.y = player_y;
    update_map(game->map, point, ressource);

    // Send a message to the player indicating successful resource collection
    char* message = "ok\n";
    write(server->clients[player_index].fd, message, strlen(message));
}

int take(Server_t *server, int fd, GameOfLifeAI *game)
{
    printf("take\n");
    process_take(server, game);
    return (0);
}

int fork_player(Server_t *server, int fd, GameOfLifeAI *game)
{
    printf("add_egg\n");
    int player_index = server->clients[server->index].player_index;
    add_egg(game, game->players[player_index]);
    char* message = "ok\n";
    write(server->clients[player_index].fd, message, strlen(message));
    return (0);
}

int incantation(Server_t *server, int fd, GameOfLifeAI *game)
{
    printf("incantation\n");
    int player_index = server->clients[server->index].player_index;
    bool succeed = check_evolution_player(game, game->players[player_index]);
    if (succeed) {
        printf("player %d evolved\n", player_index);
        char* message = "Elevation underway\n";
        char message2[100];  // Adjust the buffer size as needed
        strcpy(message2, "Current level: ");
        strcat(message, message2);
        int level = game->players[player_index]->level;
        char level_str[10];
        sprintf(level_str, "%d\n", level);
        strcat(message, level_str);

        write(server->clients[player_index].fd, message, strlen(message));
    } else {
        char* message = "ko\n";
        write(server->clients[player_index].fd, message, strlen(message));
    }
    return (0);
}

void process_move(Server_t *server, GameOfLifeAI *game, char *message)
{
    int *action = malloc(sizeof(int) * 4);
    
    if (strstr(message, "Forward") != NULL) {
        action[0] = 1;
        action[1] = 0;
        action[2] = 0;
        action[3] = 0;
    } else if (strstr(message, "Right") != NULL) {
        action[0] = 0;
        action[1] = 1;
        action[2] = 0;
        action[3] = 0;
    } else if (strstr(message, "Left") != NULL) {
        action[0] = 0;
        action[1] = 0;
        action[2] = 1;
        action[3] = 0;
    } else if (strstr(message, "None") != NULL) {
        action[0] = 0;
        action[1] = 0;
        action[2] = 0;
        action[3] = 1;
    } else {
        action[0] = 0;
        action[1] = 0;
        action[2] = 0;
        action[3] = 0;
    }
    int reward = 0;
    Point player_position = game->players[server->index]->position;
    game->players[server->index]->direction = choose_direction(action, game->players[server->index]);
    printf("player %d is moving to direction %d\n", server->index, game->players[server->index]->direction);
    Ressource *ressource = check_next_move(game, game->players[server->index], &player_position);
    printf("player position: %d %d\n", game->players[server->index]->position.x, game->players[server->index]->position.y);
    bool condition = verify_next_tile(game, game->players[server->index], player_position, &reward);
    reward += move_player(game->players[server->index], condition, ressource, player_position);
    printf("Player position: %d, %d\n", game->players[server->index]->position.x, game->players[server->index]->position.y);

    bool gameOver = check_player_state(game->players[server->index], &reward);
    if (gameOver) {
        game->players[server->index]->is_dead = 1;
        game->players[server->index]->inventory.food = 0;
        game->players[server->index]->inventory.linemate = 0;
        game->players[server->index]->inventory.deraumere = 0;
        game->players[server->index]->inventory.sibur = 0;
        game->players[server->index]->inventory.mendiane = 0;
        game->players[server->index]->inventory.phiras = 0;
        game->players[server->index]->inventory.thystame = 0;
        game->players[server->index]->direction = RIGHT;
        game->players[server->index]->level = 1;
        game->players[server->index]->position.x = 0;
        game->players[server->index]->position.y = 0;
        game->players[server->index]->team = NULL;
    }
    char *message2 = "ok\n";
    write(server->clients[server->index].fd, message2, strlen(message2));
}

int move_direction(Server_t *server, int fd, GameOfLifeAI *game)
{
    printf("player move\n");
    process_move(server, game, server->buffer);
    return (0);
}

void init_command_list(Command_list_t *list)
{
    list->command = NULL;
    list->next = NULL;
    // help, login, logout, noop, users, user, send, messages, suscribe, unsubscribe, suscribed, use, create, list, info, quit 

    list->command = malloc(sizeof(Command_t));
    list->command->command = "/help";
    list->command->func = &help;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;

    list->command = malloc(sizeof(Command_t));
    list->command->command = "/login";
    list->command->func = &login;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;

    list->command = malloc(sizeof(Command_t));
    list->command->command = "/logout";
    list->command->func = &logout;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;

    list->command = malloc(sizeof(Command_t));
    list->command->command = "/noop";
    list->command->func = &noop;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;

    list->command = malloc(sizeof(Command_t));
    list->command->command = "/users";
    list->command->func = &users;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;

    list->command = malloc(sizeof(Command_t));
    list->command->command = "/user";
    list->command->func = &user;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;

    list->command = malloc(sizeof(Command_t));
    list->command->command = "/send";
    list->command->func = &send_to;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;

    list->command = malloc(sizeof(Command_t));
    list->command->command = "/messages";
    list->command->func = &messages;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;
    
    list->command = malloc(sizeof(Command_t));
    list->command->command = "/quit";
    list->command->func = &quit;

    list->next = malloc(sizeof(Command_list_t));
    list = list->next;
    list->command = malloc(sizeof(Command_t));
    list->command->command = "/password";
    list->command->func = &password;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;

    list->command = malloc(sizeof(Command_t));
    list->command->command = "Look";
    list->command->func = &look;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;

    list->command = malloc(sizeof(Command_t));
    list->command->command = "Inventory";
    list->command->func = &inventory;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;

    list->command = malloc(sizeof(Command_t));
    list->command->command = "Broadcast";
    list->command->func = &broadcast;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;

    list->command = malloc(sizeof(Command_t));
    list->command->command = "Take";
    list->command->func = &take;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;

    list->command = malloc(sizeof(Command_t));
    list->command->command = "Move";
    list->command->func = &move_direction;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;

    list->command = malloc(sizeof(Command_t));
    list->command->command = "Fork";
    list->command->func = &fork_player;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;

    list->command = malloc(sizeof(Command_t));
    list->command->command = "Incantation";
    list->command->func = &incantation;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;

    list->command = malloc(sizeof(Command_t));
    list->command->command = "Connect_nbr";
    list->command->func = &connect_nbr;
    list->next = NULL;
}

