/*
** EPITECH PROJECT, 2021
** Inventory.h
** File description:
** Inventory header
*/

#ifndef INVENTORY_H
    #define INVENTORY_H

    #include "enum.h"

    typedef struct {
        int food;
        int linemate;
        int deraumere;
        int sibur;
        int mendiane;
        int phiras;
        int thystame;
    } Inventory;

    void init_inventory(Inventory* inventory);
    void add_resource(Inventory* inventory, Ressource resource, int number);
    void add_resources(Inventory* inventory, Ressource* resources, int* numbers, int count);
    void remove_resource(Inventory* inventory, Ressource resource, int number);
    void remove_resources(Inventory* inventory, Ressource* resources, int* numbers, int count);
    int get_resource(Inventory* inventory, Ressource resource);
    void get_all_resources(Inventory* inventory, int* resources);
    void get_rock_resources(Inventory* inventory, int* resources);
    void show_inventory(Inventory* inventory);

#endif  // INVENTORY_H
