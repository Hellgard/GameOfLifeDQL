from source.classEnum import Direction, Ressource
from source.constants import Point, font, White, Red, Black, Blue, Green, Yellow, Purple, Cyan, Brown, Grey, BLOCK_SIZE, SPEED, Forward
from source.classInventory import Inventory
import pygame
from source.classMap import Map
import time
from source.classPlayer import Player

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
            if player.Check_evolution_requirements(self.players):
                player.evolve()
                new_player = Player(Point(player.get_position().x, player.get_position().y), self.map_size.x,
                                    self.map_size.y)
                self.pending_players.append((new_player, time.time() + 5))  # the new player will appear 5 seconds later

    def add_pending_players(self):
        current_time = time.time()
        for player, appearance_time in self.pending_players.copy():
            if current_time >= appearance_time:
                self.players.append(player)
                self.map.update(player.get_position(), [Ressource.PLAYER, 9])
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
                self.timeBeforeDeath -= 1
                Player_pos = self.player.get_position_after(self.player.get_direction())
                resources = self.map.get(Player_pos)  # Get the resources on the tile the player is going to move to
                print("resources to be taken : ", resources)
                if Ressource.PLAYER not in resources:
                    actual_resources = self.map.get(self.player.get_position())  # Get the resources on the player's current tile
                    print("actual_resources : ", actual_resources)
                    # check if the there is a player on the tile and if there is more than one player on the tile
                    if Ressource.PLAYER in actual_resources:
                        tile = self.map.get(self.player.get_position())
                        tile.remove(Ressource.PLAYER)
                        print("tile : ", tile)
                        self.map.update(self.player.get_position(), tile)
                    else:
                        self.map.update(self.player.get_position(), [9])
                    self.player.move(self.player.get_direction())  # Move the player in the specified direction
                    for resource in resources:
                        if resource == Ressource.FOOD:
                            self.timeBeforeDeath += 126
                        if resource in self.RESOURCE_ACTIONS:
                            action = self.RESOURCE_ACTIONS[resource]
                            action(resource, 1)
                    self.map.update(self.player.get_position(), [Ressource.PLAYER, 9])  # Update the map with the player's new position
                    self.check_evolution()
                    self.add_pending_players()
                else:
                    actual_resources = self.map.get(self.player.get_position())  # Get the resources on the player's current tile
                    print("actual_resources : ", actual_resources)
                    # check if the there is a player on the tile and if there is more than one player on the tile
                    if Ressource.PLAYER in actual_resources:
                        tile = self.map.get(self.player.get_position())
                        tile.remove(Ressource.PLAYER)
                        print("tile : ", tile)
                        self.map.update(self.player.get_position(), tile)
                    else:
                        self.map.update(self.player.get_position(), [9])
                    self.player.move(self.player.get_direction())  # Move the player in the specified direction
                    # Update the map with the player's new position and the player/s on the tile
                    tile = self.map.get(self.player.get_position())
                    tile.append(Ressource.PLAYER)
                    self.map.update(self.player.get_position(), tile)


                self.map.draw()
                pygame.display.flip()
                self.speed = time.time()
                self.player.inventory.show()  # Show the player's inventory
                if self.timeBeforeDeath <= 0:
                    print("You died")
                    pygame.quit()
                    exit()

    def check_resources(self, resources):
        for resource in resources:
            if resource in self.RESOURCE_ACTIONS:
                action = self.RESOURCE_ACTIONS[resource]
                action(resource, 1)
