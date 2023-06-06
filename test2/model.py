import torch
import random
import numpy as np
import torch.nn as nn
import torch.nn.functional as F
from collections import deque
import torch.optim as optim
import os

class Linear_QNet(nn.Module):
    def __init__(self, input_size, hidden_size, output_size):
        super().__init__()
        self.linear1 = nn.Linear(input_size, hidden_size)
        self.linear2 = nn.Linear(hidden_size, output_size)

    def forward(self,t):
        t = F.relu(self.linear1(t))
        t = self.linear2(t)
        return t
    
    def save(self, file_name='model.pth'):
        folder_path = './model'
        if not os.path.exists(folder_path):
            os.makedirs(folder_path)
        file_name = os.path.join(folder_path, file_name)
        torch.save(self.state_dict(), file_name)

class QTrainer:
    def  __init__(self, model, lr, gamma):
        self.lr = lr
        self.gamma = gamma
        self.model = model
        self.optimizer = optim.Adam(model.parameters(), lr=self.lr)
        self.criteria = nn.MSELoss()

    def train_step(self, state, action, reward, nex_state, done):
        state = np.array(state, dtype=np.float32)
        nex_state = np.array(nex_state, dtype=np.float32)
        action = np.array(action, dtype=np.float32)
        reward = np.array(reward, dtype=np.float32)
        state = torch.tensor(state, dtype=torch.float)
        nex_state = torch.tensor(nex_state, dtype=torch.float)
        action = torch.tensor(action, dtype=torch.long)
        reward = torch.tensor(reward, dtype=torch.float)

        if len(state.shape) == 1:
            # add batch dimension
            state = torch.unsqueeze(state, 0)
            nex_state = torch.unsqueeze(nex_state, 0)
            action = torch.unsqueeze(action, 0)
            reward = torch.unsqueeze(reward, 0)
            done = (done, )
        
        pred = self.model(state)

        target = pred.clone() 
        for idx in range(len(done)):
            q_new = reward[idx]
            if not done[idx]:
                q_new = reward[idx] + self.gamma * torch.max(self.model(nex_state[idx]))

            target[idx][torch.argmax(action).item()] = q_new

        self.optimizer.zero_grad()
        loss = self.criteria(target, pred)
        loss.backward()

        self.optimizer.step()



