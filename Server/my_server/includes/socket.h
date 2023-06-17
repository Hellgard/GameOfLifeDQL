/*
** EPITECH PROJECT, 2021
** socket.h
** File description:
** header file for socket
*/

#ifndef SOCKET_H_
    #define SOCKET_H_

    #define ERROR 84
    #define MAX_CLIENTS 10

    #include <string.h>
    #include <netdb.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/select.h>
    #include <errno.h>
    #include "stdlib.h"
    #include "stdio.h"
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <stdbool.h>
    #include "../Game/include/Game.h"

    typedef struct Teams_s {
        char *name;
        char *description;
        char *uuid;
        struct Channels_s *channels;
        struct Teams_s *next;
    } Teams_t;

    typedef struct Channels_s {
        char *name;
        char *description;
        char *uuid;
        struct Threads_s *threads;
        struct Channels_s *next;
    } Channels_t;

    typedef struct Threads_s {
        char *title;
        char *message;
        char *uuid;
        struct Threads_s *next;
    } Threads_t;

    typedef struct Client_s {
        int fd;
        char *username;
        char *password;
        int logged;
        int player_id;
        int player_index;
        char *team;
        bool is_ready;
    } Client_t;

    typedef struct My_Socket_s {
        socklen_t size;
        fd_set readfs;
        fd_set allfds;
        fd_set writefds;
        int new_cfd;
        int active;
        int check;
        struct sockaddr_in s_in;
    } Socket_t;
    
    typedef struct Server_s {
        int index;
        int fd;
        int max_fd;
        int port;
        char *buffer;
        Client_t *clients;
        Teams_t *teams;
    } Server_t;

    typedef struct Command_s {
        char *command;
        int (*func)(Server_t *, int, GameOfLifeAI *);
    } Command_t;

    typedef struct Command_list_s {
        Command_t *command;
        struct Command_list_s *next;
    } Command_list_t;

    typedef struct {
        int port;
        int width;
        int height;
        char** team_names;
        int num_teams;
        int clients_per_team;
        double frequency;
    } ServerOptions_t;

    int create_socket(void);
    int configure_socket(Server_t *server);
    int init_queue(Server_t *server);
    Socket_t *connect_socket(Server_t *server);
    int server(Server_t *server, Command_list_t *command_list, ServerOptions_t *options, GameOfLifeAI *game);
    bool strContains(char *str, char *toFind);
    void init_server_null(Server_t *server);
    void init_command_list(Command_list_t *list);
#endif