/*
** EPITECH PROJECT, 2021
** utils.c
** File description:
** utils
*/

#include "socket.h"

bool strContains(char *str, char *toFind)
{
    if (strstr(str, toFind) != NULL)
        return true;
    return false;
}

int init_queue(Server_t *server)
{
    struct sockaddr_in s_in;
    socklen_t size = sizeof(s_in);

    if (getsockname(server->fd, (struct sockaddr *)&s_in, &size) == -1)
        return (ERROR);
    return (0);
}

Threads_t *init_thread(void)
{
    Threads_t *thread = malloc(sizeof(Threads_t));

    thread->title = NULL;
    thread->message = NULL;
    thread->uuid = NULL;
    thread->next = NULL;
    return (thread);
}

Channels_t *init_channel(void)
{
    Channels_t *channel = malloc(sizeof(Channels_t));

    channel->name = NULL;
    channel->description = NULL;
    channel->uuid = NULL;
    channel->threads = init_thread();
    channel->next = NULL;
    return (channel);
}

Teams_t *init_teams(void)
{
    Teams_t *team = malloc(sizeof(Teams_t));

    team->name = NULL;
    team->description = NULL;
    team->uuid = NULL;
    team->channels = init_channel();
    team->next = NULL;
    return (team);
}

void init_server_null(Server_t *server)
{
    server->clients = malloc(sizeof(Client_t) * MAX_CLIENTS);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        server->clients[i].fd = 0;
        server->clients[i].username = NULL;
        server->clients[i].password = NULL;
        server->clients[i].logged = 0;
    }
    server->fd = 0;
    server->port = 0;
    server->index = 0;
    server->max_fd = 0;
    server->teams = init_teams();
}
