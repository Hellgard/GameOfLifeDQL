from source.classEnum import Direction, Ressource
from source.constants import Point, font, White, Red, Black, Blue, Green, Yellow, Purple, Cyan, Brown, Grey, BLOCK_SIZE, FPS, Forward
from source.classInventory import Inventory
import pygame
from source.classMap import Map
import time
from source.classPlayer import Player
import random
import numpy as np

class GameOfLifeAI:
    def __init__(self, map_size):
        self.map_size = map_size
        self.clock = pygame.time.Clock()
        self.reset()
        
    def reset(self):
        self.speed = time.time()
        self.time = 0
        self.current = 0
        self.timeBeforeRegen = 20
        self.pending_players = []
        self.value = 0.1
        self.player = Player(Point(20, 20), self.map_size.x, self.map_size.y, 1, 1)
        self.players = [self.player]
        self.map = Map(self.map_size.x, self.map_size.y, self.players)
        self.evolve_Iteration = 0
        self.nearest = self.player
        self.RESOURCE_ACTIONS = {
            Ressource.FOOD: self.player.inventory.add,
            Ressource.LIMEMATE: self.player.inventory.add,
            Ressource.DERAUMERE: self.player.inventory.add,
            Ressource.SIBUR: self.player.inventory.add,
            Ressource.MENDIANE: self.player.inventory.add,
            Ressource.PHIRAS: self.player.inventory.add,
            Ressource.THYSTAME: self.player.inventory.add
        }

    def nearest_player_same_level(self, player):
        nearest = self.nearest
        for play in self.players:
            if play == player:
                continue
            if play.level == player.level:
                if nearest is self.player:
                    nearest = play
                elif player.get_distance(nearest) > player.get_distance(play):
                    nearest = play
        
        self.nearest = nearest
        # print("nearest is " + str(nearest.position))

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

        # choose one of the player that can evolve
        player = random.choice(players_) if len(players_) > 0 else self.player
        if addPlayer:
            # print ("nearest is " + str(self.nearest.position))
            self.evolve_Iteration = 50
            new_player = Player(Point(player.get_position().x, player.get_position().y), self.map_size.x,
                                self.map_size.y, len(self.players) + 1, 1)
            self.pending_players.append((new_player, self.time, players_)) # the new player will appear 2 seconds later

    def add_pending_players(self):
        current_time = self.time
        for player2, appearance_time, players in self.pending_players.copy():
            if appearance_time + 2 < current_time:
                self.players.append(player2)
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

    def check_user_input(self, player):
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    for player in self.players:
                        player.show()
                    # self.map.show()
                    pygame.quit()
                    exit()
        
    def get_players(self):
        return self.players

    def check_next_move(self, player):
        self.time += 1
        self.evolve_Iteration += 1
        Player_pos = player.get_position_after(player.get_direction())
        resources = self.map.get(Player_pos)  # Get the resources on the tile the player is going to move to
        return resources, Player_pos
           
    def verif_next_tile(self, player, posNext):
        reward = 0
        isPlayer = False
        for play in self.players:
            if play == player:
                continue
            if play.get_position() == posNext:
                isPlayer = True
                break
        if isPlayer:
            isPlayer2 = False
            for playe in self.players:
                if playe == player:
                    continue
                if playe.get_position() == player.get_position():
                    reward += 10
                    isPlayer2 = True
                    break
            if isPlayer2 == False:
                self.map.update(player.get_position(), [Ressource.EMPTY])
            return False, reward
        else:
            isPlayer2 = False
            for playe in self.players:
                if playe == player:
                    continue
                if playe.get_position() == player.get_position():
                    reward += 10
                    isPlayer2 = True
                    break
            if isPlayer2 == False:
                self.map.update(player.get_position(), [Ressource.EMPTY])
            return True, reward

    def move_player(self, player, condition, resources):
        self.RESOURCE_ACTIONS = {
            Ressource.FOOD: player.inventory.add,
            Ressource.LIMEMATE: player.inventory.add,
            Ressource.DERAUMERE: player.inventory.add,
            Ressource.SIBUR: player.inventory.add,
            Ressource.MENDIANE: player.inventory.add,
            Ressource.PHIRAS: player.inventory.add,
            Ressource.THYSTAME: player.inventory.add
        }
        self.evolve_Iteration += 1
        reward = 0
        if condition:
            player.move()  # Move the player
            for resource in resources:
                if resource == Ressource.FOOD:
                    player.timeBeforeDeath += 126
                    reward += 1
                if resource in self.RESOURCE_ACTIONS:
                    reward += 2
                    action = self.RESOURCE_ACTIONS[resource]
                    action(resource, 1)
        else:
            self.player.move()  # Move the player in the specified direction
        return reward

    def check_game_state(self):
        # check if the ressources need to be regenerated
        if self.timeBeforeRegen <= 0:
            self.timeBeforeRegen = 20
            self.map.regenerate_ressources(self.players)
        # self.player.inventory.show()  # Show the player's inventory

    def check_player_state(self, player):
        GameOver = False
        reward = 0
        if player.timeBeforeDeath <= 0:
            reward -= 100
            print("player " + str(player.id) + " died")
            GameOver = True
        return GameOver, reward

    def choose_direction(self, action, player):
        clock_wise = [Direction.RIGHT, Direction.DOWN, Direction.LEFT, Direction.UP, Direction.NONE]  # Ajout de None à la liste des directions possibles
        idx = clock_wise.index(player.get_direction())

        if np.array_equal(action, [1, 0, 0, 0]): # Si l'action est [1, 0, 0, 0], la nouvelle direction est la direction actuelle
            new_dir = clock_wise[idx]
        elif np.array_equal(action, [0, 1, 0, 0]): # Si l'action est [0, 1, 0, 0], la nouvelle direction est la direction suivante dans le sens horaire
            new_dir = clock_wise[(idx + 1) % 5]  # Utilisation du modulo 5 pour gérer le dépassement de l'indice
        elif np.array_equal(action, [0, 0, 1, 0]): # Si l'action est [0, 0, 1, 0], la nouvelle direction est la direction précédente dans le sens horaire
            new_dir = clock_wise[(idx - 1) % 5]  # Utilisation du modulo 5 pour gérer le dépassement de l'indice
        else: # Si l'action est [0, 0, 0, 1], la nouvelle direction est None
            new_dir = clock_wise[4]  # Si l'action est [0, 0, 0, 1], la nouvelle direction est None

        player.set_direction(new_dir)

    def check_resources(self, resources):
        for resource in resources:
            if resource in self.RESOURCE_ACTIONS:
                action = self.RESOURCE_ACTIONS[resource]
                action(resource, 1)

