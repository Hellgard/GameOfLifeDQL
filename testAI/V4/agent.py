import torch
import random
import numpy as np
from source2.classGame import GameOfLifeAI, Direction, Point, Ressource, Inventory, Player, Map
from collections import deque
from model import Linear_QNet, QTrainer
from helper import plot
from source2.constants import FPS

MAX_MEMORY = 100_000
BATCH_SIZE = 1000
LR = 0.001


class Agent2:
    def __init__(self):
        self.ngames = 0
        self.epsilon = 0 # randomness
        self.gamma = 0.9 # discount rate
        self.memory = deque(maxlen=MAX_MEMORY) # popleft()
        self.model = Linear_QNet(13, 256, 4)
        self.trainer = QTrainer(self.model, lr=LR, gamma=self.gamma)

    def get_state(self, game, player):
        map = game.map
        nearest = game.nearest

        direction = player.direction
        inventory = player.inventory
        position = player.position
        can_evolve = player.can_evolve()

        level = player.level
        # player can see pyramid of 3,1 blocks in front of him
        if direction == Direction.RIGHT:
            ressource1 = map.get(Point(position.x + 1, position.y))
            ressource2 = map.get(Point(position.x + 1, position.y + 1))
            ressource3 = map.get(Point(position.x + 1, position.y - 1))
            ressource4 = map.get(Point(position.x + 2, position.y))
        elif direction == Direction.LEFT:
            ressource1 = map.get(Point(position.x - 1, position.y))
            ressource2 = map.get(Point(position.x - 1, position.y + 1))
            ressource3 = map.get(Point(position.x - 1, position.y - 1))
            ressource4 = map.get(Point(position.x - 2, position.y))
        elif direction == Direction.UP:
            ressource1 = map.get(Point(position.x, position.y - 1))
            ressource2 = map.get(Point(position.x + 1, position.y - 1))
            ressource3 = map.get(Point(position.x - 1, position.y - 1))
            ressource4 = map.get(Point(position.x, position.y - 2))
        elif direction == Direction.DOWN:
            ressource1 = map.get(Point(position.x, position.y + 1))
            ressource2 = map.get(Point(position.x + 1, position.y + 1))
            ressource3 = map.get(Point(position.x - 1, position.y + 1))
            ressource4 = map.get(Point(position.x, position.y + 2))
        elif direction == Direction.NONE:
            ressource1 = None
            ressource2 = None
            ressource3 = None
            ressource4 = None

        dir_l = direction == Direction.LEFT
        dir_r = direction == Direction.RIGHT
        dir_u = direction == Direction.UP
        dir_d = direction == Direction.DOWN
        dir_n = direction == Direction.NONE

        nearest_player_pos = nearest.position

        state = [
            # is there a ressource in front of the player
            ressource1 is not None,
            ressource2 is not None,
            ressource3 is not None,
            ressource4 is not None,

            # move direction
            dir_l,
            dir_r,
            dir_u,
            dir_d,
            dir_n,

            # nearest player with same level location
            nearest_player_pos.x < position.x,
            nearest_player_pos.x > position.x,
            nearest_player_pos.y < position.y,
            nearest_player_pos.y > position.y,
        ]
        return np.array(state, dtype=int)

    def remember(self, state, action, reward, next_state, done):
        self.memory.append((state, action, reward, next_state, done))

    def train_long_memory(self):
        if len(self.memory) > BATCH_SIZE:
            mini_sample = random.sample(self.memory, BATCH_SIZE)
        else:
            mini_sample = self.memory

        states, actions, rewards, next_states, dones = zip(*mini_sample)

        self.trainer.train_step(states, actions, rewards, next_states, dones)

    def train_short_memory(self, state, action, reward, next_state, done):
        self.trainer.train_step(state, action, reward, next_state, done)

    def get_action(self, state):
        self.epsilon = 80 - self.ngames
        final_move = [0, 0, 0, 0]  # [left, right, forward, none] (Ajout de 0 pour la direction None)
        if random.randint(0, 200) < self.epsilon:
            move = random.randint(0, 3)  # Générer un entier entre 0 et 3 inclus pour inclure la direction None
            final_move[move] = 1
        else:
            state = torch.tensor(state, dtype=torch.float)
            pred = self.model(state)
            move = np.argmax(pred.detach().numpy())
            final_move[move] = 1

        return final_move

class Agent:
    def __init__(self):
        self.ngames = 0
        self.epsilon = 0 # randomness
        self.gamma = 0.9 # discount rate
        self.memory = deque(maxlen=MAX_MEMORY) # popleft()
        self.model = Linear_QNet(13, 256, 4)
        self.trainer = QTrainer(self.model, lr=LR, gamma=self.gamma)

    def get_state(self, game, player):
        map = game.map
        nearest = game.nearest

        direction = player.direction
        inventory = player.inventory
        position = player.position
        can_evolve = player.can_evolve()

        level = player.level
        # player can see pyramid of 3,1 blocks in front of him
        if direction == Direction.RIGHT:
            ressource1 = map.get(Point(position.x + 1, position.y))
            ressource2 = map.get(Point(position.x + 1, position.y + 1))
            ressource3 = map.get(Point(position.x + 1, position.y - 1))
            ressource4 = map.get(Point(position.x + 2, position.y))
        elif direction == Direction.LEFT:
            ressource1 = map.get(Point(position.x - 1, position.y))
            ressource2 = map.get(Point(position.x - 1, position.y + 1))
            ressource3 = map.get(Point(position.x - 1, position.y - 1))
            ressource4 = map.get(Point(position.x - 2, position.y))
        elif direction == Direction.UP:
            ressource1 = map.get(Point(position.x, position.y - 1))
            ressource2 = map.get(Point(position.x + 1, position.y - 1))
            ressource3 = map.get(Point(position.x - 1, position.y - 1))
            ressource4 = map.get(Point(position.x, position.y - 2))
        elif direction == Direction.DOWN:
            ressource1 = map.get(Point(position.x, position.y + 1))
            ressource2 = map.get(Point(position.x + 1, position.y + 1))
            ressource3 = map.get(Point(position.x - 1, position.y + 1))
            ressource4 = map.get(Point(position.x, position.y + 2))
        elif direction == Direction.NONE:
            ressource1 = None
            ressource2 = None
            ressource3 = None
            ressource4 = None

        dir_l = direction == Direction.LEFT
        dir_r = direction == Direction.RIGHT
        dir_u = direction == Direction.UP
        dir_d = direction == Direction.DOWN
        dir_n = direction == Direction.NONE

        nearest_player_pos = nearest.position

        state = [
            # is there a ressource in front of the player
            ressource1 is not None,
            ressource2 is not None,
            ressource3 is not None,
            ressource4 is not None,

            # move direction
            dir_l,
            dir_r,
            dir_u,
            dir_d,
            dir_n,

            # nearest player with same level location
            nearest_player_pos.x < position.x,
            nearest_player_pos.x > position.x,
            nearest_player_pos.y < position.y,
            nearest_player_pos.y > position.y,
        ]
        return np.array(state, dtype=int)

    def remember(self, state, action, reward, next_state, done):
        self.memory.append((state, action, reward, next_state, done))

    def train_long_memory(self):
        if len(self.memory) > BATCH_SIZE:
            mini_sample = random.sample(self.memory, BATCH_SIZE)
        else:
            mini_sample = self.memory

        states, actions, rewards, next_states, dones = zip(*mini_sample)

        self.trainer.train_step(states, actions, rewards, next_states, dones)

    def train_short_memory(self, state, action, reward, next_state, done):
        self.trainer.train_step(state, action, reward, next_state, done)

    def get_action(self, state):
        self.epsilon = 80 - self.ngames
        final_move = [0, 0, 0, 0]  # [left, right, forward, none] (Ajout de 0 pour la direction None)
        if random.randint(0, 200) < self.epsilon:
            move = random.randint(0, 3)  # Générer un entier entre 0 et 3 inclus pour inclure la direction None
            final_move[move] = 1
        else:
            state = torch.tensor(state, dtype=torch.float)
            pred = self.model(state)
            move = np.argmax(pred.detach().numpy())
            final_move[move] = 1

        return final_move

# test it from saved model
def train():
    plot_scores = []
    plot_mean_scores = []
    total_score = 0
    total_reward = 0
    record_reward = 0
    record = 0
    agent2 = Agent2()
    agent = Agent()
    agent.model.load_state_dict(torch.load('model/model1.pth'))
    agent.model.eval()
    game = GameOfLifeAI(Point(60, 60))
    while True:
        for player in game.players:
            player_to_train = game.player
            if player is not player_to_train:
                Ressources = [Ressource.FOOD, Ressource.THYSTAME, Ressource.LIMEMATE, Ressource.DERAUMERE, Ressource.SIBUR, Ressource.MENDIANE, Ressource.PHIRAS]
                numbers = [100, 100, 100, 100, 100, 100, 100]
                # player.inventory.adds(Ressources, numbers)
                old_state = agent.get_state(game, player)
                final_move = agent.get_action(old_state)

                game.check_user_input(player)

                game.choose_direction(final_move, player)
                ressources, pos = game.check_next_move(player)
                condition = game.verif_next_tile(player, pos)
                reward = game.move_player(player=player, condition=condition, resources=ressources)
                game.nearest_player_same_level(player)
                done, reward2 = game.check_player_state(player)
                reward += reward2
                
                new_state = agent.get_state(game, player)
                agent.train_short_memory(old_state, final_move, reward, new_state, done)
                agent.remember(old_state, final_move, reward, new_state, done)
                
                if done and player:
                    game.players.remove(player)
        player = game.player
        reward = 0
        old_state = agent2.get_state(game, player)
        final_move = agent2.get_action(old_state)

        game.check_user_input(player)

        game.choose_direction(final_move, player)
        ressources, pos = game.check_next_move(player)
        condition = game.verif_next_tile(player, pos)
        reward = game.move_player(player=player, condition=condition, resources=ressources)
        game.nearest_player_same_level(player)
        done, reward2 = game.check_player_state(player)
        reward += reward2
        score = player.level
        
        game.timeBeforeRegen -= 1
        game.check_game_state()
        
        game.map.draw(game.get_players())

        reward += game.check_evolution()
        total_reward += reward
        game.add_pending_players()
        new_state = agent2.get_state(game, player)
        agent2.train_short_memory(old_state, final_move, reward, new_state, done)
        agent2.remember(old_state, final_move, reward, new_state, done)
        

        game.clock.tick(FPS)
        if done:
            game.reset()
            agent2.ngames += 1
            agent2.train_long_memory()

            if total_reward > record_reward:
                record_reward = total_reward
                agent2.model.save()

            if score > record:
                record = score

            print('Game', agent2.ngames, 'Score', score, 'Record', record, 'Reward:', total_reward, 'Record Reward:', record_reward)
            plot_scores.append(score)
            total_score += score
            mean_score = total_score / agent2.ngames
            plot_mean_scores.append(mean_score)
            plot(plot_scores, plot_mean_scores)
            total_reward = 0
 


if __name__ == '__main__':
    train()