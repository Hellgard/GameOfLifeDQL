/*
** EPITECH PROJECT, 2021
** main.c
** File description:
** main
*/

#include "socket.h"

int error(int ac, char **av)
{
    if (ac != 3) {
        printf("Usage: ./server port path\n");
        return (84);
    }
    if (atoi(av[1]) < 1024 || atoi(av[1]) > 65535) {
        printf("Port must be between 1024 and 65535\n");
        return (84);
    }
    if (access(av[2], F_OK) == -1) {
        printf("Path doesn't exist\n");
        return (84);
    }
    return (0);
}

int main(int ac, char **av)
{
    Server_t *servers = malloc(sizeof(Server_t));
    init_server_null(servers);
    servers->port = atoi(av[1]);
    Command_list_t *command_list = malloc(sizeof(Command_list_t *) * 15);
    init_command_list(command_list);
    if (command_list == NULL)
        return -1;
    if (error(ac, av) == 84)
        return (84);
    server(servers, command_list);
    return (0);
}
