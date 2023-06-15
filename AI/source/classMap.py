import random
from source.classEnum import Ressource, Density
from source.constants import Point, font, White, Red, Black, Blue, Green, Yellow, Purple, Cyan, Brown, Grey, BLOCK_SIZE, Forward
import pygame

class Map:
    def __init__(self, width=60, height=60, players=[]):
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
        self.generate_map(players)

    def generate_map(self, players):
        for y in range(self.height):
            for x in range(self.width):
                tile = [Ressource.EMPTY]
                isPlayer = False
                for player in players:
                    if player.position.x == x and player.position.y == y:
                        tile.append(Ressource.PLAYER)
                        isPlayer = True
                if isPlayer == False:
                    tile = self.generate_ressources()
                self.map[y][x] = tile
    
    def regenerate_ressources(self, players):
        isPlayer = False
        for y in range(self.height):
            for x in range(self.width):
                for player in players:
                    if player.position.x == x and player.position.y == y:
                        isPlayer = True
                if isPlayer == True:
                    isPlayer = False
                else:
                    self.map[y][x] = self.generate_ressources()
        
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
    
    def draw(self, players):
        for y in range(self.height):
            for x in range(self.width):
                tile = self.map[y][x]
                isPlayer = False
                for player in players:
                    if player.position.x == x and player.position.y == y:
                        isPlayer = True
                if isPlayer == True:
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
        
    def show(self):
        for y in range(self.height):
            for x in range(self.width):
                print(self.map[y][x], end='')
            print()
