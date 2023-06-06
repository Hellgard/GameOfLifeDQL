import pygame
import random
import time
from enum import Enum
from collections import namedtuple
import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import os
import matplotlib.pyplot as plt
from IPython import display

pygame.init()
font = pygame.font.Font('data/arial.ttf', 25)

class Direction(Enum):
    RIGHT = 1
    LEFT = 2
    UP = 3
    DOWN = 4
    NONE = 5

Point = namedtuple('Point', 'x, y')

White = (255, 255, 255)
Red = (255, 0, 0)
Black = (0, 0, 0) 
Blue = (0, 0, 255) 
Green = (0, 255, 0) 
Yellow = (255, 255, 0) 
Purple = (255, 0, 255) 
Cyan = (0, 255, 255) 
Brown = (165, 42, 42)
Grey = (128, 128, 128)

BLOCK_SIZE = 20
SPEED = 100
Forward = 100

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

class EvolutionRequirements_1(Enum):
    PlayerLevel1 = 1
    Linemate = 1
    Deraumere = 0
    Sibur = 0
    Mendiane = 0
    Phiras = 0
    Thystame = 0

class EvolutionRequirements_2(Enum):
    PlayerLevel2 = 2
    Linemate = 1
    Deraumere = 1
    Sibur = 1
    Mendiane = 0
    Phiras = 0
    Thystame = 0

class EvolutionRequirements_3(Enum):
    PlayerLevel3 = 2
    Linemate = 2
    Deraumere = 0
    Sibur = 1
    Mendiane = 0
    Phiras = 2
    Thystame = 0

class EvolutionRequirements_4(Enum):
    PlayerLevel4 = 4
    Linemate = 1
    Deraumere = 1
    Sibur = 2
    Mendiane = 0
    Phiras = 1
    Thystame = 0

class EvolutionRequirements_5(Enum):
    PlayerLevel5 = 4
    Linemate = 1
    Deraumere = 2
    Sibur = 1
    Mendiane = 3
    Phiras = 0
    Thystame = 0

class EvolutionRequirements_6(Enum):
    PlayerLevel6 = 6
    Linemate = 1
    Deraumere = 2
    Sibur = 3
    Mendiane = 0
    Phiras = 1
    Thystame = 0

class EvolutionRequirements_7(Enum):
    PlayerLevel7 = 6
    Linemate = 2
    Deraumere = 2
    Sibur = 2
    Mendiane = 2
    Phiras = 2
    Thystame = 1

class Actions(Enum):
    feed = 1
    looking_for_ressources = 2
    collect_ressources = 3

class Map:
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.map = [[0 for x in range(width)] for y in range(height)]
        self.density_food = Density.Food
        self.density_linemate = Density.Linemate
        self.density_deraumere = Density.Deraumere
        self.density_sibur = Density.Sibur
        self.density_mendiane = Density.Mendiane
        self.density_phiras = Density.Phiras
        self.density_thystame = Density.Thystame
        self.child = []
        self.screen = pygame.display.set_mode((self.width * BLOCK_SIZE, self.height * BLOCK_SIZE))
        self.screen.fill(Black)
        self.generate_map()

    def generate_map(self):
        for y in range(self.height):
            for x in range(self.width):
                if x == 20 and y == 20:
                    self.map[y][x] = [Ressource.PLAYER, Ressource.EMPTY]
                elif x == 20 and y == 10:
                    self.map[y][x] = [Ressource.PLAYER, Ressource.EMPTY]
                elif x == 10 and y == 20:
                    self.map[y][x] = [Ressource.PLAYER, Ressource.EMPTY, Ressource.PLAYER, Ressource.PLAYER ]
                elif x == 10 and y == 30:
                    self.map[y][x] = [Ressource.PLAYER, Ressource.EMPTY, Ressource.PLAYER, Ressource.PLAYER, Ressource.PLAYER, Ressource.PLAYER ]
                else:
                    self.map[y][x] = self.generate_ressources()
    
    def regenerate_ressources(self):
        for y in range(self.height):
            for x in range(self.width):
                if Ressource.PLAYER in self.map[y][x]:
                    continue
                else:
                    self.map[y][x] = self.generate_ressources()

    def generate_ressource(self):
        rand = random.random()
        if rand <= self.density_thystame.value:
            return Ressource.THYSTAME
        elif rand <= self.density_phiras.value:
            return Ressource.PHIRAS
        elif rand <= self.density_mendiane.value:
            rand2 = random.random()
            if rand2 <= 0.5:
                return Ressource.MENDIANE
            else:
                return Ressource.SIBUR
        elif rand <= self.density_deraumere.value:
            return Ressource.DERAUMERE
        elif rand <= self.density_linemate.value:
            return Ressource.LIMEMATE
        elif rand <= self.density_food.value:
            return Ressource.FOOD
        else:
            return Ressource.EMPTY
        
    def generate_ressources(self):
        ressources = []
        densities = {
            Ressource.FOOD: Density.Food.value,
            Ressource.LIMEMATE: Density.Linemate.value,
            Ressource.DERAUMERE: Density.Deraumere.value,
            Ressource.SIBUR: Density.Sibur.value,
            Ressource.MENDIANE: Density.Mendiane.value,
            Ressource.PHIRAS: Density.Phiras.value,
            Ressource.THYSTAME: Density.Thystame.value
        }
        
        # Determine if the tile should be empty
        if random.random() <= (1 - sum(densities.values())):
            return ressources

        for ressource, density in densities.items():
            if random.random() <= density:
                ressources.append(ressource)

        return ressources
    
    def draw(self):
        for y in range(self.height):
            for x in range(self.width):
                tile = self.map[y][x]
                if Ressource.PLAYER in tile:
                    pygame.draw.rect(self.screen, Black, pygame.Rect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))
                else:
                    color = self.get_predominant_color(tile)
                    pygame.draw.rect(self.screen, color, pygame.Rect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))
        pygame.display.flip()

    
    def get_predominant_color(self, tile):
        seed = random.random()
        resource_counts = {}
        for resource in tile:
            resource_counts[resource] = resource_counts.get(resource, 0) + 1

        max_count = 0
        predominant_resources = []
        for resource, count in resource_counts.items():
            if count > max_count:
                max_count = count
                predominant_resources = [resource]
            elif count == max_count:
                predominant_resources.append(resource)

        # If there is more than one predominant resource, choose one the first predominant resource
        if len(predominant_resources) > 0:
            predominant_color = self.get_color(predominant_resources[0])
            if len(predominant_resources) > 1:
                predominant_color = self.get_color(predominant_resources[0])
        else:
            # since there is no predominant resource, choose the first resource
            if len(tile) > 0:
                predominant_color = self.get_color(tile[0])
            else:
                predominant_color = Red

        return predominant_color
    
    def get_color(self, resource):
        if resource == Ressource.FOOD:
            return Green
        elif resource == Ressource.LIMEMATE:
            return Grey
        elif resource == Ressource.DERAUMERE:
            return Brown
        elif resource == Ressource.SIBUR:
            return Blue
        elif resource == Ressource.MENDIANE:
            return Purple
        elif resource == Ressource.PHIRAS:
            return Yellow
        elif resource == Ressource.THYSTAME:
            return White
        elif resource == Ressource.PLAYER:
            return Black
        else:
            return Red

    def update(self, point, ressource):
        if point.x < self.width and point.y < self.height:
            self.map[point.y][point.x] = ressource
        elif point.x > self.width:
            self.map[point.y][0] = ressource
        elif point.y > self.height:
            self.map[0][point.x] = ressource
        else:
            self.map[0][0] = ressource

    def get(self, point):
        if point.x < self.width and point.y < self.height:
            return self.map[point.y][point.x]
        elif point.x > self.width:
            return self.map[point.y][0]
        elif point.y > self.height:
            return self.map[0][point.x]
        else:
            return self.map[0][0]


class Inventory:
    def __init__(self):
        self.food = 0
        self.linemate = 0
        self.deraumere = 0
        self.sibur = 0
        self.mendiane = 0
        self.phiras = 0
        self.thystame = 0
        # EvolutionRequirements[level][ressource][numberrequiered] 

    def add(self, ressource, number):
        if ressource == Ressource.FOOD:
            self.food += number
        elif ressource == Ressource.LIMEMATE:
            self.linemate += number
        elif ressource == Ressource.DERAUMERE:
            self.deraumere += number
        elif ressource == Ressource.SIBUR:
            self.sibur += number
        elif ressource == Ressource.MENDIANE:
            self.mendiane += number
        elif ressource == Ressource.PHIRAS:
            self.phiras += number
        elif ressource == Ressource.THYSTAME:
            self.thystame += number
    
    def remove(self, ressource, number):
        if ressource == Ressource.FOOD:
            self.food -= number
        elif ressource == Ressource.LIMEMATE:
            self.linemate -= number
        elif ressource == Ressource.DERAUMERE:
            self.deraumere -= number
        elif ressource == Ressource.SIBUR:
            self.sibur -= number
        elif ressource == Ressource.MENDIANE:
            self.mendiane -= number
        elif ressource == Ressource.PHIRAS:
            self.phiras -= number
        elif ressource == Ressource.THYSTAME:
            self.thystame -= number

    def removes(self, ressources, numbers):
        # ressources = [Ressource.FOOD, Ressource.LIMEMATE, Ressource.DERAUMERE, Ressource.SIBUR, Ressource.MENDIANE, Ressource.PHIRAS, Ressource.THYSTAME]
        # numbers = [1, 1, 1, 1, 1, 1, 1]
        for i in range(len(ressources)):
            self.remove(ressources[i], numbers[i])

    def get(self, ressource):
        if ressource == Ressource.FOOD:
            return self.food
        elif ressource == Ressource.LIMEMATE:
            return self.linemate
        elif ressource == Ressource.DERAUMERE:
            return self.deraumere
        elif ressource == Ressource.SIBUR:
            return self.sibur
        elif ressource == Ressource.MENDIANE:
            return self.mendiane
        elif ressource == Ressource.PHIRAS:
            return self.phiras
        elif ressource == Ressource.THYSTAME:
            return self.thystame
        
    def get_all(self):
        return [self.food, self.linemate, self.deraumere, self.sibur, self.mendiane, self.phiras, self.thystame]
    
    def get_rocks(self):
        return [self.linemate, self.deraumere, self.sibur, self.mendiane, self.phiras, self.thystame]
    
    def show(self):
        print("Food: " + str(self.food) + "\nLinemate: " + str(self.linemate) + "\nDeraumere: " + str(self.deraumere) + "\nSibur: " + str(self.sibur) + "\nMendiane: " + str(self.mendiane) + "\nPhiras: " + str(self.phiras) + "\nThystame: " + str(self.thystame))

class Player:
    def __init__(self, Point, width, height, rand, level):
        self.levelRequirements = [ [1, 1, 0, 0, 0, 0, 0], [2, 1, 1, 1, 0, 0, 0], [3, 2, 0, 1, 0, 2, 0], [4, 1, 1, 2, 0, 1, 0], [5, 1, 2, 1, 3, 0, 0], [6, 1, 2, 3, 0, 1, 0], [7, 2, 2, 2, 2, 2, 1] ]
        self.playerRequirements = [1, 2, 2, 4, 4, 6, 6]
        self.width = width
        self.height = height
        self.position = Point
        self.direction = Direction.RIGHT
        self.inventory = Inventory()
        self.childs = []
        self.level = level
        self.id = random.randint(0, 100000) - rand

    def play_sound(self, sound_file):
        pygame.mixer.init()
        pygame.mixer.music.load(sound_file)
        pygame.mixer.music.play()


    def Check_evolution_requirements(self, players):
        if self.level == 8:
            return False
        
        number_of_players = 1
        if self.level > 1:
            for other_player in players:
                if other_player.get_id() != self.get_id():
                    # print ("other player is not self")
                    # print (other_player.get_position())
                    # print (self.get_position())
                    if other_player.get_position() == self.get_position():
                        # print ("other player on same tile")
                        # print (other_player.get_level())
                        # print (self.levelRequirements[self.level - 1][0])
                        if other_player.get_level() == self.levelRequirements[self.level - 1][0]:
                            # print ("other player has higher level")
                            number_of_players += 1
        if number_of_players < self.playerRequirements[self.level - 1]:
            return False
        
        for i in range(7):
            if self.inventory.get_all()[i] < self.levelRequirements[self.level - 1][i]:
                if i == 0:
                    continue
                # print ("requirement not met: " + Ressource(i + 1).name + " " + str(self.inventory.get_all()[i]) + " < " + str(self.levelRequirements[self.level - 1][i]))
                return False
        
        # delete rocks from inventory
        soundfile = "data/vorbis.mp3"
        self.play_sound(soundfile)
        list = self.levelRequirements[self.level - 1][1:7]
        # print (list)
        self.inventory.removes([Ressource.LIMEMATE, Ressource.DERAUMERE, Ressource.SIBUR, Ressource.MENDIANE, Ressource.PHIRAS, Ressource.THYSTAME], list)
        return True
    
    def get_id(self):
        return self.id
    
    def move(self):
        direction = self.get_direction()


        if direction == Direction.NONE:
            return
        if direction == Direction.UP:
            self.position = Point(self.position.x, self.position.y - 1)
        elif direction == Direction.DOWN:
            self.position = Point(self.position.x, self.position.y + 1)
        elif direction == Direction.LEFT:
            self.position = Point(self.position.x - 1, self.position.y)
        elif direction == Direction.RIGHT:
            self.position = Point(self.position.x + 1, self.position.y)
        
        if self.position.x < 0:
            self.position = Point(self.width - 1, self.position.y)
        elif self.position.x >= self.width:
            self.position = Point(0, self.position.y)
        elif self.position.y < 0:
            self.position = Point(self.position.x, self.height - 1)
        elif self.position.y >= self.height:
            self.position = Point(self.position.x, 0)

    def get_inventory(self):
        return self.inventory
    
    def get_level(self):
        return self.level
    
    def get_direction(self):
        return self.direction
    
    def get_position(self):
        return self.position
    
    def get_position_after(self, direction):
        if direction == Direction.UP:
            return Point(self.position.x, self.position.y - 1)
        elif direction == Direction.DOWN:
            return Point(self.position.x, self.position.y + 1)
        elif direction == Direction.LEFT:
            return Point(self.position.x - 1, self.position.y)
        elif direction == Direction.RIGHT:
            return Point(self.position.x + 1, self.position.y)
        else:
            return self.position
    
    def set_direction(self, direction):
        self.direction = direction

    def set_position(self, position):
        self.position = position

    def set_level(self, level):
        self.level = level

    def evolve(self):
        self.level += 1

    def set_child(self, child):
        self.childs.append(child)

    def get_childs(self):
        return self.childs
    
    def get_child(self, index):
        return self.childs[index]
    
    def show(self):
        print ("------------------\n")
        print ("Player")
        print ("id: " + str(self.id))
        print ("level: " + str(self.level))
        print ("position: " + str(self.position.x) + ", " + str(self.position.y))
        print ("direction: " + str(self.direction))
        print ("inventory: " + str(self.inventory.get_all()))
        print ("childs: " + str(len(self.childs)))
        print ("\n------------------")

class GameOfLifeAI:
    def __init__(self, map_size):
        self.map_size = map_size
        self.clock = pygame.time.Clock()
        self.reset()
        
    def reset(self):
        self.map = Map(self.map_size.x, self.map_size.y)
        self.speed = time.time()
        self.timeBeforeDeath = 1260 / Forward
        self.time = 0
        self.current = 0
        self.timeBeforeRegen = 20
        self.pending_players = []
        self.value = 0.1
        self.player = Player(Point(20, 20), self.map_size.x, self.map_size.y, 1, 1)
        self.player_level2 = Player(Point(20, 10), self.map_size.x, self.map_size.y, 2, 2)
        self.player_level4 = Player(Point(10, 20), self.map_size.x, self.map_size.y, 4, 4)
        self.player_level4_2 = Player(Point(10, 20), self.map_size.x, self.map_size.y, 4.5, 4)
        self.player_level4_3 = Player(Point(10, 20), self.map_size.x, self.map_size.y, 4.8, 4)
        self.player_level6 = Player(Point(10, 30), self.map_size.x, self.map_size.y, 6, 6)
        self.player_level6_2 = Player(Point(10, 30), self.map_size.x, self.map_size.y, 6.2, 6)
        self.player_level6_3 = Player(Point(10, 30), self.map_size.x, self.map_size.y, 6.3, 6)
        self.player_level6_4 = Player(Point(10, 30), self.map_size.x, self.map_size.y, 6.4, 6)
        self.player_level6_5 = Player(Point(10, 30), self.map_size.x, self.map_size.y, 6.5, 6)
        self.players = [self.player, self.player_level2, self.player_level4, self.player_level4_2, self.player_level4_3, self.player_level6, self.player_level6_2, self.player_level6_3, self.player_level6_4, self.player_level6_5]
        for play in self.players:
            if play == self.player:
                continue
            play.inventory.add(Ressource.FOOD, 100)
            play.inventory.add(Ressource.LIMEMATE, 100)
            play.inventory.add(Ressource.DERAUMERE, 100)
            play.inventory.add(Ressource.SIBUR, 100)
            play.inventory.add(Ressource.MENDIANE, 100)
            play.inventory.add(Ressource.PHIRAS, 100)
            play.inventory.add(Ressource.THYSTAME, 100)
        self.evolve_Iteration = 0
        self.RESOURCE_ACTIONS = {
            Ressource.FOOD: self.player.inventory.add,
            Ressource.LIMEMATE: self.player.inventory.add,
            Ressource.DERAUMERE: self.player.inventory.add,
            Ressource.SIBUR: self.player.inventory.add,
            Ressource.MENDIANE: self.player.inventory.add,
            Ressource.PHIRAS: self.player.inventory.add,
            Ressource.THYSTAME: self.player.inventory.add
        }


    def check_evolution(self):
        reward = 0
        addPlayer = False
        players_ = []
        for player2 in self.players:
            # player2.show()
            if player2.Check_evolution_requirements(self.players):
                print("Player " + str(player2.id) + " evolved")
                addPlayer = True
                players_.append(player2)

        for play in players_:
            reward += play.level
            play.evolve()
        player = self.player
        if addPlayer:
            self.evolve_Iteration = 50
            new_player = Player(Point(player.get_position().x, player.get_position().y), self.map_size.x,
                                self.map_size.y, len(self.players) + 1, 1)
            self.pending_players.append((new_player, self.time, players_)) # the new player will appear 2 seconds later
        return reward

    def add_pending_players(self):
        current_time = self.time
        for player2, appearance_time, players in self.pending_players.copy():
            if appearance_time + 2 < current_time:
                self.players.append(player2)
                tile = self.map.get(player2.get_position())
                tile.append(Ressource.PLAYER)
                self.map.update(player2.get_position(), tile)
                for player1 in players:
                    player1.set_child(player2)
                self.pending_players.remove((player2, appearance_time, players))

    def add_player(self, player):
        self.players.append(player)

    def add_ressource(self, ressource):
        self.ressources.append(ressource)

    def get_player(self, index):
        return self.players[index]

    def get_ressource(self, index):
        return self.ressources[index]

    def play_step(self, action):
        
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    # print pos of all players
                    for player in self.players:
                        print(player.show())
                    pygame.quit()
                    exit()
        done, score, reward = self.move(action)
        self.clock.tick(SPEED)
        return done, score, reward
    
    def move(self, action):
        self.choose_direction(action, self.player)
        reward = 0
        self.time += 1
        self.evolve_Iteration += 1
        self.timeBeforeRegen -= 1
        self.timeBeforeDeath -= 1
        Player_pos = self.player.get_position_after(self.player.get_direction())
        resources = self.map.get(Player_pos)  # Get the resources on the tile the player is going to move to
        # print("resources to be taken : ", resources)

        # check if the player can move to the tile, and if he can, move him
        reward = self.verif_and_move(resources)

        GameOver = False
        if self.timeBeforeDeath <= 0:
            print("You died")
            reward = -30
            GameOver = True
            return GameOver, self.player.get_level(), reward
        if self.evolve_Iteration >= 500:
            print("You died")
            reward = -20
            GameOver = True
            return GameOver, self.player.get_level(), reward
        
        # check if the ressources need to be regenerated
        if self.timeBeforeRegen <= 0:
            self.timeBeforeRegen = 20
            self.map.regenerate_ressources()

        self.map.draw()
        pygame.display.flip()
        # self.player.inventory.show()  # Show the player's inventory
        return GameOver, self.player.get_level(), reward
           
    def verif_and_move(self, resources):
        reward = 0
        if Ressource.PLAYER not in resources:
            actual_resources = self.map.get(self.player.get_position())  # Get the resources on the player's current tile
            # print("actual_resources : ", actual_resources)
            # check if the there is a player on the tile and if there is more than one player on the tile
            if Ressource.PLAYER in actual_resources:
                reward += 1
                reward = self.check_evolution()
                tile = self.map.get(self.player.get_position())
                tile.remove(Ressource.PLAYER)
                # print("tile : ", tile)
                self.map.update(self.player.get_position(), tile)
            else:
                self.map.update(self.player.get_position(), [Ressource.EMPTY])
            self.player.move()  # Move the player
            for resource in resources:
                # if resource == Ressource.EMPTY:
                #     reward -= 1
                if resource == Ressource.FOOD:
                    self.timeBeforeDeath += 126
                    reward += 1
                if resource in self.RESOURCE_ACTIONS:
                    action = self.RESOURCE_ACTIONS[resource]
                    action(resource, 1)
                    if resource != Ressource.FOOD and resource != Ressource.EMPTY:
                        reward += 2
            self.map.update(self.player.get_position(), [Ressource.PLAYER, Ressource.EMPTY])  # Update the map with the player's new position
            self.add_pending_players()
        else:
            actual_resources = self.map.get(self.player.get_position())  # Get the resources on the player's current tile
            # print("actual_resources : ", actual_resources)
            # check if the there is a player on the tile and if there is more than one player on the tile
            if Ressource.PLAYER in actual_resources:
                tile = self.map.get(self.player.get_position())
                tile.remove(Ressource.PLAYER)
                # print("tile : ", tile)
                self.map.update(self.player.get_position(), tile)
            else:
                self.map.update(self.player.get_position(), [Ressource.EMPTY])
            self.player.move()  # Move the player in the specified direction
            # Update the map with the player's new position and the player/s on the tile
            tile = self.map.get(self.player.get_position())
            tile.append(Ressource.PLAYER)
            self.map.update(self.player.get_position(), tile)
        return reward

    def choose_direction(self, action, player):
        clock_wise = [Direction.RIGHT, Direction.LEFT, Direction.UP, Direction.DOWN]
        idx = clock_wise.index(player.get_direction())


        if np.array_equal(action, [1, 0, 0]):
            new_dir = clock_wise[idx]
        elif np.array_equal(action, [0, 1, 0]):
            new_dir = clock_wise[(idx + 1) % 4]
        else:
            new_dir = clock_wise[(idx - 1) % 4]

        # print("new dir: " + str(new_dir))
        player.set_direction(new_dir)

    def check_resources(self, resources):
        for resource in resources:
            if resource in self.RESOURCE_ACTIONS:
                action = self.RESOURCE_ACTIONS[resource]
                action(resource, 1)

