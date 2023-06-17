/*
** EPITECH PROJECT, 2021
** main.c
** File description:
** main
*/

#include "socket.h"
#include "../Game/include/Game.h"

void print_usage() {
    printf("USAGE: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq\n");
    printf("port is the port number\n");
    printf("width is the width of the world\n");
    printf("height is the height of the world\n");
    printf("nameX is the name of the team X\n");
    printf("clientsNb is the number of authorized clients per team\n");
    printf("freq is the reciprocal of time unit for execution of actions\n");
}

void parse_port(ServerOptions_t* options, int argc, char* argv[], int arg_index) {
    options->port = atoi(argv[arg_index]);
}

void parse_width(ServerOptions_t* options, int argc, char* argv[], int arg_index) {
    options->width = atoi(argv[arg_index]);
}

void parse_height(ServerOptions_t* options, int argc, char* argv[], int arg_index) {
    options->height = atoi(argv[arg_index]);
}

void parse_clients_per_team(ServerOptions_t* options, int argc, char* argv[], int arg_index) {
    options->clients_per_team = atoi(argv[arg_index]);
}

void parse_frequency(ServerOptions_t* options, int argc, char* argv[], int arg_index) {
    options->frequency = atof(argv[arg_index]);
}

void parse_team_names(ServerOptions_t* options, int argc, char* argv[], int start_index) {
    int num_teams = 0;
    int i;
    for (i = start_index; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "-f") == 0) {
            i++; // Skip the value of -c or -f
            continue;
        }
        num_teams++;
    }

    options->team_names = malloc(num_teams * sizeof(char*));
    options->num_teams = num_teams;

    int team_index = 0;
    for (i = start_index; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "-f") == 0) {
            i++; // Skip the value of -c or -f
            continue;
        }
        options->team_names[team_index] = strdup(argv[i]);
        team_index++;
    }
}

typedef void (*ParseFunction)(ServerOptions_t*, int, char*[], int);

int find_option_index(const char* arg) {
    const char* options[] = {"-p", "-x", "-y", "-n", "-c", "-f"};
    int num_options = sizeof(options) / sizeof(options[0]);
    int i;
    for (i = 0; i < num_options; i++) {
        if (strcmp(arg, options[i]) == 0) {
            return i;
        }
    }
    return -1;
}

void parse_arguments(int argc, char* argv[], ServerOptions_t* options) {
    options->port = 0;
    options->width = 0;
    options->height = 0;
    options->team_names = NULL;
    options->num_teams = 0;
    options->clients_per_team = 0;
    options->frequency = 0.0;

    ParseFunction parse_functions[] = {parse_port, parse_width, parse_height, parse_team_names, parse_clients_per_team, parse_frequency};
    int i;
    for (i = 1; i < argc; i++) {
        int option_index = find_option_index(argv[i]);
        if (option_index != -1 && i + 1 < argc) {
            parse_functions[option_index](options, argc, argv, i + 1);
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Error: Invalid argument or missing argument value.\n");
            print_usage();
            exit(1);
        }
    }
}

void free_options(ServerOptions_t* options) {
    int i;
    for (i = 0; i < options->num_teams; i++) {
        free(options->team_names[i]);
    }
    free(options->team_names);
}

int main(int ac, char **av) {
    ServerOptions_t options;
    parse_arguments(ac, av, &options);
    Server_t *servers = malloc(sizeof(Server_t));
    init_server_null(servers);
    servers->port = options.port;
    GameOfLifeAI *game;
    game = create_game_of_life_ai(options.width, options.height, options.num_teams, options.clients_per_team, options.frequency, options.team_names);

    printf("Port: %d\n", servers->port);
    printf("Width: %d\n", options.width);
    printf("Height: %d\n", options.height);
    printf("Number of teams: %d\n", options.num_teams);
    printf("Clients per team: %d\n", options.clients_per_team);
    printf("Frequency: %f\n", options.frequency);
    printf("Team names: ");
    int i;
    for (i = 0; i < options.num_teams; i++) {
        printf("%s ", options.team_names[i]);
    }
    printf("\n");

    if (options.num_teams <= 0) {
        fprintf(stderr, "Error: At least one team name must be provided.\n");
        print_usage();
        free_options(&options);
        return 1;
    }

    Command_list_t *command_list = malloc(sizeof(Command_list_t) * 15);
    if (command_list == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for command list.\n");
        free_options(&options);
        return 1;
    }
    init_command_list(command_list);

    server(servers, command_list, &options, game);

    free_options(&options);
    free(command_list);
    free(servers);

    return 0;
}
