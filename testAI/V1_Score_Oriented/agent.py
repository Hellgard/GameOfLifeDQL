import torch
import random
import numpy as np
from GameOfLifeAI import GameOfLifeAI, Direction, Point, Ressource, Inventory, Player, Map
from collections import deque
from model import Linear_QNet, QTrainer
from helper import plot

MAX_MEMORY = 100_000
BATCH_SIZE = 1000
LR = 0.001

class Agent:
    def __init__(self):
        self.ngames = 0
        self.epsilon = 0 # randomness
        self.gamma = 0.9 # discount rate
        self.memory = deque(maxlen=MAX_MEMORY) # popleft()
        self.model = Linear_QNet(12, 256, 3)
        self.trainer = QTrainer(self.model, lr=LR, gamma=self.gamma)

    def get_state(self, game):
        player = game.player
        map = game.map
        nearest = game.nearest

        direction = player.direction
        inventory = player.inventory
        position = player.position

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

        dir_l = direction == Direction.LEFT
        dir_r = direction == Direction.RIGHT
        dir_u = direction == Direction.UP
        dir_d = direction == Direction.DOWN

        nearest_player_pos = nearest.position

        state = [
            # is there a ressource in front of the player
            ressource1 is not None,
            ressource2 is not None,
            ressource3 is not None,
            ressource4 is not None,

            # # nearest player location
            # nearest_player_pos.x < position.x,
            # nearest_player_pos.x > position.x,
            # nearest_player_pos.y < position.y,
            # nearest_player_pos.y > position.y,
        
            # move direction
            dir_l,
            dir_r,
            dir_u,
            dir_d,

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
        final_move = [0, 0, 0]
        if random.randint(0, 200) < self.epsilon:
            move = random.randint(0, 2)
            final_move[move] = 1

        else:
            state = torch.tensor(state, dtype=torch.float)
            pred = self.model(state)
            move = np.argmax(pred.detach().numpy())
            final_move[move] = 1

        return final_move

# test it from saved model
def test():
    plot_scores = []
    plot_mean_scores = []
    total_score = 0
    total_reward = 0
    record = 0
    agent = Agent()
    agent.model.load_state_dict(torch.load('model.pth'))
    agent.model.eval()
    game = GameOfLifeAI(Point(40, 40))
    while True:
        old_state = agent.get_state(game)

        final_move = agent.get_action(old_state)

        done, score, reward = game.play_step(final_move)
        new_state = agent.get_state(game)
        total_reward += reward

        if done:
            game.reset()
            agent.ngames += 1

            if score > record:
                record = score
                agent.model
 
def train():
    plot_scores = []
    plot_mean_scores = []
    total_score = 0
    total_reward = 0
    record = 0
    agent = Agent()
    game = GameOfLifeAI(Point(40, 40))
    while True:
        old_state = agent.get_state(game)

        final_move = agent.get_action(old_state)

        done, score, reward = game.play_step(final_move)
        new_state = agent.get_state(game)
        total_reward += reward

        agent.train_short_memory(old_state, final_move, reward, new_state, done)

        agent.remember(old_state, final_move, reward, new_state, done)

        if done:
            game.reset()
            agent.ngames += 1
            agent.train_long_memory()

            if score > record:
                record = score
                agent.model.save()

            print('Game', agent.ngames, 'Score', score, 'Record', record, 'Reward:', total_reward)
            plot_scores.append(score)
            total_score += score
            mean_score = total_score / agent.ngames

            plot_mean_scores.append(mean_score)
            plot(plot_scores, plot_mean_scores)
            total_reward = 0

if __name__ == '__main__':
    train()