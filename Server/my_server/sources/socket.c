/*
** EPITECH PROJECT, 2021
** socket.c
** File description:
** socket
*/

#include "socket.h"

#define BUFFER_SIZE 1024

int create_socket(void)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd == -1) {
        perror("socket");
        return -1;
    }
    return fd;
}

int configure_socket(Server_t *server)
{
    struct sockaddr_in s_in = {0};

    s_in.sin_family = AF_INET;
    s_in.sin_port = htons(server->port);
    s_in.sin_addr.s_addr = INADDR_ANY;
    if (bind(server->fd, (const struct sockaddr *)&s_in, sizeof(s_in)) == -1) {
        perror("bind");
        return -1;
    }
    return 0;
}

int new_user(Server_t *server, Socket_t *socket, GameOfLifeAI *game)
{
    socket->new_cfd = accept(server->fd
    , (struct sockaddr *)&socket->s_in, &socket->size);
    if (socket->new_cfd < 0) {
        perror("accept");
        return 0;
    }
    printf("Connection from %s:%d\n", inet_ntoa(socket->s_in.sin_addr)
    , ntohs(socket->s_in.sin_port));
    for (server->index = 0; server->index < MAX_CLIENTS; server->index++) {
        if (server->clients[server->index].fd == 0) {
            server->clients[server->index].fd = socket->new_cfd;
            server->clients[server->index].is_ready = 0;
            break;
        }
    }
    FD_SET(socket->new_cfd, &socket->allfds);
    if (socket->new_cfd > server->max_fd)
        server->max_fd = socket->new_cfd;
    char* teams_slot;
    int total_length = 0;

    for (int i = 0; i < game->nb_team; i++) {
        int team_max_childs = 0; // Total max_childs for players of the same team

        // Calculate the total max_childs for players of the same team
        team_max_childs += game->teams_slot[i];

        // Calculate the total length of the concatenated strings
        total_length += strlen(game->players[i]->team) + 9; // Length of " slots\n"

        // Add the current team information to teams_slot
        if (i == 0) {
            teams_slot = malloc(total_length + 1); // +1 for the null terminator
            sprintf(teams_slot, "%s %d slots\n", game->teams_name[i], team_max_childs);
        } else {
            teams_slot = realloc(teams_slot, total_length + 1); // +1 for the null terminator
            sprintf(teams_slot, "%s%s %d slots\n", teams_slot, game->teams_name[i], team_max_childs);
        }
    }
    // char *test = strdup("WELCOME - Chose your team:\n");
    // test = strcpy(test, teams_slot);
    
    write(socket->new_cfd, teams_slot, strlen(teams_slot));
    free(teams_slot);
    return 1;

}

void command(Server_t *server, Socket_t *socket, Command_list_t *command_list, GameOfLifeAI *game)
{
    for (server->index = 0; server->index < MAX_CLIENTS; server->index++) {
        if (FD_ISSET(server->clients[server->index].fd, &socket->readfs) && server->clients[server->index].is_ready == 1) {
            server->buffer = malloc(sizeof(char) * 1024);
            server->buffer[1024] = "\n";
            int valread = read(server->clients[server->index].fd, server->buffer, 1024);
            if (valread == 0) {
                printf("Client %d disconnected unexpectedly \n", server->clients[server->index].fd);
                close(server->clients[server->index].fd);
                FD_CLR(server->clients[server->index].fd, &socket->allfds);
                server->clients[server->index].fd = 0;
                break;
            }
            char *buffer = malloc(sizeof(char) * 1024);
            buffer = server->buffer;
            buffer[valread] = '\0';
            int isCommand = 0;
            printf("Client %d said: %s\n", server->clients[server->index].fd, buffer);
            for (command_list; command_list; command_list = command_list->next) {
                if (strContains(buffer, command_list->command->command) == 1) {
                    server->buffer = buffer;
                    command_list->command->func(server, server->clients[server->index].fd, game);
                    printf("command: %s\n", command_list->command->command);
                    isCommand = 1;
                    break;
                }
            }
            if (isCommand == 0)
                write(server->clients[server->index].fd, "500 Unknown command\n", 20);
            free(buffer);
        } else if (FD_ISSET(server->clients[server->index].fd, &socket->readfs) && server->clients[server->index].is_ready == 0) {
            server->buffer = malloc(sizeof(char) * 1024);
            server->buffer[1024] = "\n";
            int valread = read(server->clients[server->index].fd, server->buffer, 1024);
            if (valread == 0) {
                printf("Client %d disconnected unexpectedly \n", server->clients[server->index].fd);
                close(server->clients[server->index].fd);
                FD_CLR(server->clients[server->index].fd, &socket->allfds);
                server->clients[server->index].fd = 0;
                break;
            }
            char *buffer = malloc(sizeof(char) * 1024);
            buffer = server->buffer;
            buffer[valread] = '\0';
            int isCommand = 0;
            int i = 0;
            printf("Client %d said: %s\n", server->clients[server->index].fd, buffer);
            for (; i < game->nb_team; i++) {
                int team_max_childs = game->teams_slot[i];
                if (strContains(buffer, game->teams_name[i]) == 1) {
                    server->buffer = buffer;
                    isCommand = 1;
                    if (team_max_childs == 0) {
                        write(server->clients[server->index].fd, "this team is full\n", 18);
                        isCommand = 2;
                        break;
                    }
                    server->clients[server->index].team = game->teams_name[i];
                }
            }
            if (isCommand == 0)
                write(server->clients[server->index].fd, "you need to choose a team\nType TEAMNAME\n", 41);
            else if (isCommand == 1){
                server->clients[server->index].is_ready = 1;

                // choose a player from the open slots in the chosen team, player that is not already taken are is_egg == 1
                printf("Current: %d\n", game->current);
                for (int i = 0; i < game->current; i++) {
                    if (strcmp(game->players[i]->team, server->clients[server->index].team) == 0 && game->players[i]->is_egg == 1) {
                        printf("Player %d is now ready to play\n", game->players[i]->id);
                        server->clients[server->index].player_id = game->players[i]->id;
                        server->clients[server->index].player_index = i;
                        game->players[i]->is_egg = 0;
                        for (int j = 0; j < game->nb_team; j++) {
                            if (strcmp(game->teams_name[j], server->clients[server->index].team) == 0) {
                                game->teams_slot[j]--;
                                break;
                            }
                        }
                        break;
                    }
                }
                write(server->clients[server->index].fd, "ok, you are ready to play with your team\n", 41);
            }
            free(buffer);
        }
    }
}

// connect_socket multiple clients, using select
Socket_t *connect_socket(Server_t *server)
{
    Socket_t *socket = malloc(sizeof(Socket_t));
    socket->size = sizeof(socket->s_in);
    server->max_fd = server->fd;// maximum filedescriptor for select
    socket->new_cfd = 0;
    server->index = 0;
    socket->active = 0;
    socket->check = 0;

    FD_ZERO(&socket->allfds);
    FD_SET(server->fd, &socket->allfds);
    return socket;
}

int loop_server(Socket_t *socket, Server_t *server, Command_list_t *command_list, GameOfLifeAI *game)
{
    while (1) {
        socket->readfs = socket->allfds;
        socket->active = select(server->max_fd + 1, 
            &socket->readfs, NULL, NULL, NULL);
        if (socket->active < 0) {
            perror("select");
            return -1;
        }
        if (FD_ISSET(server->fd, &socket->readfs)) {
            if (new_user(server, socket, game) == 0)
                return -1;
        } else {
            command(server, socket, command_list, game);
            draw_map(game->map, "map.txt", game->players, game->current);
        }
    }
    return 0;
}

int server(Server_t *server, Command_list_t *command_list, ServerOptions_t *options, GameOfLifeAI *game)
{
    server->fd = create_socket();
    if (server->fd == -1)
        return (ERROR);
    if (configure_socket(server) == -1)
        return (ERROR);
    if (listen(server->fd, 10) == -1) {
        perror("listen");
        return (ERROR);
    }
    if (init_queue(server) == -1)
        return (ERROR);
    Socket_t *socket = connect_socket(server);
    if (loop_server(socket, server, command_list, game) == -1)
        return (ERROR);
    return 0;
}
