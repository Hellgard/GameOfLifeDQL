from enum import Enum

class Direction(Enum):
    RIGHT = 1
    LEFT = 2
    UP = 3
    DOWN = 4
    NONE = 5

class Ressource(Enum):
    FOOD = 1
    LIMEMATE = 2
    DERAUMERE = 3
    SIBUR = 4
    MENDIANE = 5
    PHIRAS = 6
    THYSTAME = 7
    PLAYER = 8
    EMPTY = 9

class Density(Enum):
    Thystame = 0.05
    Phiras = 0.08
    Mendiane = 0.1
    Sibur = 0.1
    Deraumere = 0.15
    Linemate = 0.3
    Food = 0.5

class Actions(Enum):
    feed = 1
    looking_for_ressources = 2
    collect_ressources = 3