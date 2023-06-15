#include <stdio.h>
#include "../include/Inventory.h"

void init_inventory(Inventory* inventory) {
    inventory->food = 0;
    inventory->linemate = 0;
    inventory->deraumere = 0;
    inventory->sibur = 0;
    inventory->mendiane = 0;
    inventory->phiras = 0;
    inventory->thystame = 0;
}

void add_resource(Inventory* inventory, Ressource resource, int number) {
    switch (resource) {
        case Ressource_FOOD:
            inventory->food += number;
            break;
        case Ressource_LIMEMATE:
            inventory->linemate += number;
            break;
        case Ressource_DERAUMERE:
            inventory->deraumere += number;
            break;
        case Ressource_SIBUR:
            inventory->sibur += number;
            break;
        case Ressource_MENDIANE:
            inventory->mendiane += number;
            break;
        case Ressource_PHIRAS:
            inventory->phiras += number;
            break;
        case Ressource_THYSTAME:
            inventory->thystame += number;
            break;
        default:
            break;
    }
}

void add_resources(Inventory* inventory, Ressource* resources, int* numbers, int count) {
    for (int i = 0; i < count; i++) {
        add_resource(inventory, resources[i], numbers[i]);
    }
}

void remove_resource(Inventory* inventory, Ressource resource, int number) {
    switch (resource) {
        case Ressource_FOOD:
            inventory->food -= number;
            break;
        case Ressource_LIMEMATE:
            inventory->linemate -= number;
            break;
        case Ressource_DERAUMERE:
            inventory->deraumere -= number;
            break;
        case Ressource_SIBUR:
            inventory->sibur -= number;
            break;
        case Ressource_MENDIANE:
            inventory->mendiane -= number;
            break;
        case Ressource_PHIRAS:
            inventory->phiras -= number;
            break;
        case Ressource_THYSTAME:
            inventory->thystame -= number;
            break;
        default:
            break;
    }
}

void remove_resources(Inventory* inventory, Ressource* resources, int* numbers, int count) {
    for (int i = 0; i < count; i++) {
        remove_resource(inventory, resources[i], numbers[i]);
    }
}

int get_resource(Inventory* inventory, Ressource resource) {
    switch (resource) {
        case Ressource_FOOD:
            return inventory->food;
        case Ressource_LIMEMATE:
            return inventory->linemate;
        case Ressource_DERAUMERE:
            return inventory->deraumere;
        case Ressource_SIBUR:
            return inventory->sibur;
        case Ressource_MENDIANE:
            return inventory->mendiane;
        case Ressource_PHIRAS:
            return inventory->phiras;
        case Ressource_THYSTAME:
            return inventory->thystame;
        default:
            return 0;
    }
}

void get_all_resources(Inventory* inventory, int* resources) {
    resources[0] = inventory->food;
    resources[1] = inventory->linemate;
    resources[2] = inventory->deraumere;
    resources[3] = inventory->sibur;
    resources[4] = inventory->mendiane;
    resources[5] = inventory->phiras;
    resources[6] = inventory->thystame;
}

void get_rock_resources(Inventory* inventory, int* resources) {
    resources[0] = inventory->linemate;
    resources[1] = inventory->deraumere;
    resources[2] = inventory->sibur;
    resources[3] = inventory->mendiane;
    resources[4] = inventory->phiras;
    resources[5] = inventory->thystame;
}

void show_inventory(Inventory* inventory) {
    printf("Food: %d\n", inventory->food);
    printf("Linemate: %d\n", inventory->linemate);
    printf("Deraumere: %d\n", inventory->deraumere);
    printf("Sibur: %d\n", inventory->sibur);
    printf("Mendiane: %d\n", inventory->mendiane);
    printf("Phiras: %d\n", inventory->phiras);
    printf("Thystame: %d\n", inventory->thystame);
}