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
        self.player = Player(Point(20, 20), map_size.x, map_size.y, 1, 1)
        self.player_level2 = Player(Point(20, 10), map_size.x, map_size.y, 2, 2)
        self.player_level3 = Player(Point(20, 30), map_size.x, map_size.y, 3, 3)
        self.player_level3_2 = Player(Point(20, 30), map_size.x, map_size.y, 3.5, 3)
        self.player_level3_3 = Player(Point(20, 30), map_size.x, map_size.y, 3.8, 3)
        self.player_level4 = Player(Point(10, 20), map_size.x, map_size.y, 4, 4)
        self.player_level4_2 = Player(Point(10, 20), map_size.x, map_size.y, 4.5, 4)
        self.player_level4_3 = Player(Point(10, 20), map_size.x, map_size.y, 4.8, 4)
        self.player_level5 = Player(Point(10, 10), map_size.x, map_size.y, 5, 5)
        self.player_level5_2 = Player(Point(10, 10), map_size.x, map_size.y, 5.2, 5)
        self.player_level5_3 = Player(Point(10, 10), map_size.x, map_size.y, 5.3, 5)
        self.player_level5_4 = Player(Point(10, 10), map_size.x, map_size.y, 5.4, 5)
        self.player_level5_5 = Player(Point(10, 10), map_size.x, map_size.y, 5.5, 5)
        self.player_level6 = Player(Point(10, 30), map_size.x, map_size.y, 6, 6)
        self.player_level6_2 = Player(Point(10, 30), map_size.x, map_size.y, 6.2, 6)
        self.player_level6_3 = Player(Point(10, 30), map_size.x, map_size.y, 6.3, 6)
        self.player_level6_4 = Player(Point(10, 30), map_size.x, map_size.y, 6.4, 6)
        self.player_level6_5 = Player(Point(10, 30), map_size.x, map_size.y, 6.5, 6)
        self.player_level7 = Player(Point(30, 20), map_size.x, map_size.y, 7, 7)
        self.player_level7_2 = Player(Point(30, 20), map_size.x, map_size.y, 7.2, 7)
        self.player_level7_3 = Player(Point(30, 20), map_size.x, map_size.y, 7.3, 7)
        self.player_level7_4 = Player(Point(30, 20), map_size.x, map_size.y, 7.4, 7)
        self.player_level7_5 = Player(Point(30, 20), map_size.x, map_size.y, 7.5, 7)
        self.player_level8 = Player(Point(30, 10), map_size.x, map_size.y, 8, 8)
        self.player_level8_2 = Player(Point(30, 10), map_size.x, map_size.y, 8.2, 8)
        self.player_level8_3 = Player(Point(30, 10), map_size.x, map_size.y, 8.3, 8)
        self.player_level8_4 = Player(Point(30, 10), map_size.x, map_size.y, 8.4, 8)
        self.player_level8_5 = Player(Point(30, 10), map_size.x, map_size.y, 8.5, 8)
        self.players = [self.player, self.player_level2, self.player_level3, self.player_level3_2, self.player_level3_3, self.player_level4, self.player_level4_2, self.player_level4_3, self.player_level5, self.player_level5_2, self.player_level5_3, self.player_level5_4, self.player_level5_5, self.player_level6, self.player_level6_2, self.player_level6_3, self.player_level6_4, self.player_level6_5, self.player_level7, self.player_level7_2, self.player_level7_3, self.player_level7_4, self.player_level7_5, self.player_level8, self.player_level8_2, self.player_level8_3, self.player_level8_4, self.player_level8_5]
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
                                    self.map_size.y, len(self.players) + 1, 1)
                self.pending_players.append((new_player, time.time() + 5, player)) # the new player will appear 5 seconds later

    def add_pending_players(self):
        current_time = time.time()
        for player2, appearance_time, player in self.pending_players.copy():
            if current_time >= appearance_time:
                self.players.append(player2)
                self.map.update(player2.get_position(), [Ressource.PLAYER, 9])
                player.set_child(player2)
                self.pending_players.remove((player2, appearance_time, player))

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
                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_ESCAPE:
                        # print pos of all players
                        for player in self.players:
                            print(player.show())
                        pygame.quit()
                        exit()
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
                else:
                    self.player.set_direction(Direction.NONE)

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
                        self.check_evolution()
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

                print("player level : ", self.player.get_level())
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
