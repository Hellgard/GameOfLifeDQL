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

int register_user(Server_t *server, int fd)
{
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
    if (username_error_handle(found, user, server) == 0)
        return (0);
    server->clients[server->index].logged = 3;
    server->clients[server->index].username = strdup(user);
    char *register_user = "230 Username correct, proceed with pass.\n";
    write(server->clients[server->index].fd, register_user,
    strlen(register_user));
    return (0);
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

int password(Server_t *server, int fd)
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

int login(Server_t *server, int fd)
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

int logout(Server_t *server, int fd)
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

int noop(Server_t *server, int fd)
{
    char *noop = "200 Command okay.\n";

    write(server->clients[server->index].fd, noop, strlen(noop));
    return (0);
}

int users(Server_t *server, int fd)
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


int user(Server_t *server, int fd)
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

int send_to(Server_t *server, int fd)
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

int messages(Server_t *server, int fd)
{
    char *messages = "530 Not logged in.\n";

    write(server->clients[server->index].fd, messages, strlen(messages));
    return (0);
}

int suscribe(Server_t *server, int fd)
{
    char *suscribe = "530 Not logged in.\n";

    write(server->clients[server->index].fd, suscribe, strlen(suscribe));
    return (0);
}

int unsubscribe(Server_t *server, int fd)
{
    char *unsubscribe = "530 Not logged in.\n";

    write(server->clients[server->index].fd, unsubscribe, strlen(unsubscribe));
    return (0);
}

int suscribed(Server_t *server, int fd)
{
    char *suscribed = "530 Not logged in.\n";

    write(server->clients[server->index].fd, suscribed, strlen(suscribed));
    return (0);
}

int use(Server_t *server, int fd)
{
    char *use = "530 Not logged in.\n";

    write(server->clients[server->index].fd, use, strlen(use));
    return (0);
}

int create(Server_t *server, int fd)
{
    char *create = "530 Not logged in.\n";

    write(server->clients[server->index].fd, create, strlen(create));
    return (0);
}

int list(Server_t *server, int fd)
{
    char *list = "530 Not logged in.\n";

    write(server->clients[server->index].fd, list, strlen(list));
    return (0);
}

int info(Server_t *server, int fd)
{
    char *info = "530 Not logged in.\n";

    write(server->clients[server->index].fd, info, strlen(info));
    return (0);
}

int quit(Server_t *server, int fd)
{
    char *quit = "221 Service closing control connection.\n";

    write(server->clients[server->index].fd, quit, strlen(quit));
    close(server->clients[server->index].fd);
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
    list->command->command = "/suscribe";
    list->command->func = &suscribe;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;
    list->command = malloc(sizeof(Command_t));
    list->command->command = "/unsubscribe";
    list->command->func = &unsubscribe;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;
    list->command = malloc(sizeof(Command_t));
    list->command->command = "/suscribed";
    list->command->func = &suscribed;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;
    list->command = malloc(sizeof(Command_t));
    list->command->command = "/use";

    list->command->func = &use;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;
    list->command = malloc(sizeof(Command_t));
    list->command->command = "/create";
    list->command->func = &create;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;

    list->command = malloc(sizeof(Command_t));
    list->command->command = "/list";
    list->command->func = &list;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;
    list->command = malloc(sizeof(Command_t));
    list->command->command = "/info";
    list->command->func = &info;
    list->next = malloc(sizeof(Command_list_t));
    list = list->next;
    list->command = malloc(sizeof(Command_t));
    list->command->command = "/quit";
    list->command->func = &quit;

    list->next = malloc(sizeof(Command_list_t));
    list = list->next;
    list->command = malloc(sizeof(Command_t));
    list->command->command = "register";
    list->command->func = &register_user;

    list->next = malloc(sizeof(Command_list_t));
    list = list->next;
    list->command = malloc(sizeof(Command_t));
    list->command->command = "password";
    list->command->func = &password;

    list->next = NULL;
}
