import pygame
import random
import time
from enum import Enum
from collections import namedtuple
import numpy as np

pygame.init()
font = pygame.font.Font('data/arial.ttf', 25)

class Direction(Enum):
    RIGHT = 1
    LEFT = 2
    UP = 3
    DOWN = 4

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

BLOCK_SIZE = 10
SPEED = 1
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
                    self.map[y][x] = Ressource.PLAYER
                else:
                    self.map[y][x] = self.generate_ressource()
    
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
            return 0
            
    
    def draw(self):
        for y in range(self.height):
            for x in range(self.width):
                if self.map[y][x] == Ressource.PLAYER:
                    pygame.draw.rect(self.screen, Black, pygame.Rect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))
                elif self.map[y][x] == Ressource.FOOD:
                    # print("Food")
                    pygame.draw.rect(self.screen, Green, pygame.Rect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))
                elif self.map[y][x] == Ressource.LIMEMATE:
                    # print("Linemate")
                    pygame.draw.rect(self.screen, Yellow, pygame.Rect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))
                elif self.map[y][x] == Ressource.DERAUMERE:
                    # print("Deraumere")
                    pygame.draw.rect(self.screen, Blue, pygame.Rect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))
                elif self.map[y][x] == Ressource.SIBUR:
                    # print("Sibur")
                    pygame.draw.rect(self.screen, Brown, pygame.Rect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))
                elif self.map[y][x] == Ressource.MENDIANE:
                    # print("Mendiane")
                    pygame.draw.rect(self.screen, Purple, pygame.Rect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))
                elif self.map[y][x] == Ressource.PHIRAS:
                    # print("Phiras")
                    pygame.draw.rect(self.screen, Cyan, pygame.Rect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))
                elif self.map[y][x] == Ressource.THYSTAME:
                    # print("Thystame")
                    pygame.draw.rect(self.screen, White, pygame.Rect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))
                else:
                    # print("Empty")
                    pygame.draw.rect(self.screen, Red, pygame.Rect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))
                # print ("x = " + str(x) + " y = " + str(y), " self.map[y][x] = " + str(self.map[y][x]))
        pygame.display.flip()

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
    def __init__(self, Point, width, height):
        self.levelRequirements = [ [1, 1, 0, 0, 0, 0, 0], [2, 1, 1, 1, 0, 0, 0], [2, 2, 0, 1, 0, 2, 0], [4, 1, 1, 2, 0, 1, 0], [4, 1, 2, 1, 3, 0, 0], [6, 1, 2, 3, 0, 1, 0], [6, 2, 2, 2, 2, 2, 1] ]
        self.width = width
        self.height = height
        self.position = Point
        self.direction = Direction.RIGHT
        self.inventory = Inventory()
        self.childs = []
        self.level = 1

    def play_sound(self, sound_file):
        pygame.mixer.init()
        pygame.mixer.music.load(sound_file)
        pygame.mixer.music.play()


    def Check_evolution_requirements(self, players):
        if self.level == 8:
            return False
        print ("Check_evolution_requirements")
        print (self.levelRequirements[self.level - 1])
        print (self.inventory.get_all())
        for i in range(7):
            if self.inventory.get_all()[i] < self.levelRequirements[self.level - 1][i]:
                if i == 0:
                    j = self.levelRequirements[self.level - 1][0]
                    # check if other players are level > j
                    for player in players:
                        if player.level > j:
                            return False
                return False
        
        # delete rocks from inventory
        soundfile = "data/vorbis.mp3"
        self.play_sound(soundfile)
        list = self.levelRequirements[self.level - 1][1:7]
        print (list)
        self.inventory.removes([Ressource.LIMEMATE, Ressource.DERAUMERE, Ressource.SIBUR, Ressource.MENDIANE, Ressource.PHIRAS, Ressource.THYSTAME], list)
        return True
    
    def move(self, direction):
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



class GameOfLife:
    def __init__(self, map_size):
        self.map_size = map_size
        self.player = Player(Point(20, 20), map_size.x, map_size.y)
        self.players = [self.player]
        self.map = Map(map_size.x, map_size.y) 
        self.speed = time.time()
        self.timeBeforeDeath = 1260 / Forward
        self.time = time.time()
        self.RESOURCE_ACTIONS = {
            Ressource.FOOD: self.player.inventory.add,
            Ressource.LIMEMATE: self.player.inventory.add,
            Ressource.DERAUMERE: self.player.inventory.add,
            Ressource.SIBUR: self.player.inventory.add,
            Ressource.MENDIANE: self.player.inventory.add,
            Ressource.PHIRAS: self.player.inventory.add,
            Ressource.THYSTAME: self.player.inventory.add
        }
        self.pending_players = []

    def check_evolution(self):
        for player in self.players:
            print ("check_evolution of player")
            print (player)
            if player.Check_evolution_requirements(self.players):
                player.evolve()
                new_player = Player(Point(player.get_position().x, player.get_position().y), self.map_size.x, self.map_size.y)
                self.pending_players.append((new_player, time.time() + 5))  # the new player will appear 5 seconds later
    
    def add_pending_players(self):
        current_time = time.time()
        for player, appearance_time in self.pending_players.copy():
            if current_time >= appearance_time:
                self.players.append(player)
                self.map.update(player.get_position(), Ressource.PLAYER)
                self.pending_players.remove((player, appearance_time))
                player.set_child(player)

    def add_player(self, player):
        self.players.append(player)

    def add_ressource(self, ressource):
        self.ressources.append(ressource)

    def get_player(self, index):
        return self.players[index]
    
    def get_ressource(self, index):
        return self.ressources[index]
    
    def game(self):

        Speed = SPEED
        Value = 0.1
        while True:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    exit()
                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_LEFT:
                        self.player.set_direction(Direction.LEFT)
                    elif event.key == pygame.K_RIGHT:
                        self.player.set_direction(Direction.RIGHT)
                    elif event.key == pygame.K_UP:
                        self.player.set_direction(Direction.UP)
                    elif event.key == pygame.K_DOWN:
                        self.player.set_direction(Direction.DOWN)
                    elif event.key == pygame.K_SPACE:
                        Speed -= Value
                        Value += 0.1
                    elif event.key == pygame.K_RETURN:
                        Speed += Value
                        Value -= 0.1

            if time.time() - self.speed >= Speed:
                self.check_evolution()
                self.add_pending_players()
                self.timeBeforeDeath -= 1
                Player_pos = self.player.get_position_after(self.player.get_direction())
                ressource = self.map.get(Player_pos) # get la ressource sur laquelle le joueur est
                if ressource != Ressource.PLAYER:
                    self.map.update(self.player.get_position(), 9) # 9 = empty, remplacer la position du joueur par un empty
                    self.player.move(self.player.get_direction()) # bouger le joueur dans la direction
                    if ressource == Ressource.FOOD:
                        self.timeBeforeDeath += 126
                    print(ressource)
                    self.check_resources(ressource) # check si la ressource est une ressource
                    self.map.update(self.player.get_position(), Ressource.PLAYER) # update la map avec la position du joueur
                self.map.draw()
                pygame.display.flip()
                self.speed = time.time()
                self.player.inventory.show() # afficher l'inventaire du joueUr
                if self.timeBeforeDeath <= 0:
                    print("You died")
                    pygame.quit()
                    exit()
                

    
    def check_resources(self, resource):
        action = self.RESOURCE_ACTIONS.get(resource)
        if action:
            action(resource, 1)

def __main__():
    game = GameOfLife(Point(40, 40))
    print("Game of life")
    game.map.draw()
    game.game()

__main__()
