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

int new_user(Server_t *server, Socket_t *socket)
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
            break;
        }
    }
    FD_SET(socket->new_cfd, &socket->allfds);
    if (socket->new_cfd > server->max_fd)
        server->max_fd = socket->new_cfd;
    write(socket->new_cfd, "220 Service ready for new user.\n", 32);
    return 1;
}

void command(Server_t *server, Socket_t *socket, Command_list_t *command_list)
{
    for (server->index = 0; server->index < MAX_CLIENTS; server->index++) {
        if (FD_ISSET(server->clients[server->index].fd, &socket->readfs)) {
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
                    command_list->command->func(server, server->clients[server->index].fd);
                    printf("command: %s\n", command_list->command->command);
                    isCommand = 1;
                    break;
                }
            }
            if (isCommand == 0)
                write(server->clients[server->index].fd, "500 Unknown command.\n", 21);
            free(server->buffer);
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

int loop_server(Socket_t *socket, Server_t *server, Command_list_t *command_list)
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
            if (new_user(server, socket) == 0)
                return -1;
        } else
            command(server, socket, command_list);
    }
    return 0;
}

int server(Server_t *server, Command_list_t *command_list)
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
    if (loop_server(socket, server, command_list) == -1)
        return (ERROR);
    return 0;
}
