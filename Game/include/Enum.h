/*
** EPITECH PROJECT, 2021
** Enum.h
** File description:
** Enum
*/

#ifndef ENUM_H
    #define ENUM_H

    typedef enum {
        RIGHT = 1,
        LEFT = 2,
        UP = 3,
        DOWN = 4,
        NONE = 5
    } Direction;

    typedef enum {
        Ressource_FOOD = 1,
        Ressource_LIMEMATE = 2,
        Ressource_DERAUMERE = 3,
        Ressource_SIBUR = 4,
        Ressource_MENDIANE = 5,
        Ressource_PHIRAS = 6,
        Ressource_THYSTAME = 7,
        Ressource_PLAYER = 8,
        Ressource_EMPTY = 9
    } Ressource;

    #define Density_thystame  0.05
    #define Density_phiras  0.08
    #define Density_mendiane  0.1
    #define Density_sibur  0.1
    #define Density_deraumere  0.15
    #define Density_linemate  0.3
    #define Density_food  0.5

#endif
