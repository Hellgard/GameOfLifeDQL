from source.classEnum import Direction, Ressource
from source.constants import Point, font, White, Red, Black, Blue, Green, Yellow, Purple, Cyan, Brown, Grey, BLOCK_SIZE, SPEED, Forward
from source.classInventory import Inventory
import pygame
import random



class Player:
    def __init__(self, Point, width, height, rand, level):
        self.levelRequirements = [ [1, 1, 0, 0, 0, 0, 0], [2, 1, 1, 1, 0, 0, 0], [2, 2, 0, 1, 0, 2, 0], [4, 1, 1, 2, 0, 1, 0], [4, 1, 2, 1, 3, 0, 0], [6, 1, 2, 3, 0, 1, 0], [6, 2, 2, 2, 2, 2, 1] ]
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
            # print ("Check_evolution_requirements")
            # print (self.levelRequirements[self.level - 1])
            # print (self.inventory.get_all())
            for other_player in players:
                if other_player.get_id() != self.get_id():
                    # print ("other player is not self")
                    # print (other_player.get_position())
                    # print (self.get_position())
                    if other_player.get_position() == self.get_position():
                        # print ("other player on same tile")
                        # print (other_player.get_level())
                        # print (self.levelRequirements[self.level - 1][0])
                        if other_player.get_level() >= self.levelRequirements[self.level - 1][0]:
                            # print ("other player has higher level")
                            number_of_players += 1
        
        if number_of_players < self.playerRequirements[self.level - 1]:
            return False
        
        for i in range(7):
            if self.inventory.get_all()[i] < self.levelRequirements[self.level - 1][i]:
                if i == 0:
                    continue
                return False
        
        # delete rocks from inventory
        soundfile = "vorbis.mp3"
        self.play_sound(soundfile)
        list = self.levelRequirements[self.level - 1][1:7]
        # print (list)
        self.inventory.removes([Ressource.LIMEMATE, Ressource.DERAUMERE, Ressource.SIBUR, Ressource.MENDIANE, Ressource.PHIRAS, Ressource.THYSTAME], list)
        return True
    
    def get_id(self):
        return self.id
    
    def move(self, direction):
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