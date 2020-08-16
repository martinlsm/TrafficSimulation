import pickle

import tensorflow as tf
from tensorflow import keras
import numpy as np

import one_car_env as env

def build_dqn(learning_rate, state_dim, action_dim, fst_dense, snd_dense):
    model = keras.Sequential([
        keras.layers.Input(state_dim),
        keras.layers.Dense(fst_dense, activation='relu'),
        keras.layers.Dense(snd_dense, activation='relu'),
        keras.layers.Dense(action_dim, activation='linear')
    ])
    model.compile(optimizer=keras.optimizers.Adam(learning_rate=learning_rate),
                  loss='mean_squared_error')
    return model

class ReplayBuffer():
    def __init__(self, memory_size, state_dim):
        self.memory_size = memory_size
        self.memory_counter = 0
        self.from_state_memory = np.zeros((memory_size, state_dim))
        self.to_state_memory = np.zeros((memory_size, state_dim))
        self.action_memory = np.zeros(memory_size, dtype=np.int32)
        self.reward_memory = np.zeros(memory_size, dtype=np.int32)
        self.terminal_state_memory = np.zeros(memory_size, dtype=np.bool)

    def store_transition(self, from_state, to_state, action, reward, done):
        index = self.memory_counter % self.memory_size
        self.from_state_memory[index] = from_state
        self.to_state_memory[index] = to_state
        self.action_memory[index] = action
        self.reward_memory[index] = reward
        self.terminal_state_memory[index] = done
        self.memory_counter += 1

    def get_batch(self, batch_size):
        current_memory_size = min(self.memory_size, self.memory_counter)
        batch_indices = np.random.choice(current_memory_size, batch_size, replace=False)

        from_states = self.from_state_memory[batch_indices]
        to_states = self.to_state_memory[batch_indices]
        actions = self.action_memory[batch_indices]
        rewards = self.reward_memory[batch_indices]
        terminals = self.terminal_state_memory[batch_indices]

        return from_states, to_states, actions, rewards, terminals

class CarAgent():
    def __init__(self, learning_rate, gamma, state_dim, action_dim,
                 eps_start, eps_decrement, eps_end, memory_size):
        self.learning_rate = learning_rate
        self.gamma = gamma
        self.state_dim = state_dim
        self.action_dim = action_dim
        self.eps = eps_start
        self.eps_decrement = eps_decrement
        self.eps_end = eps_end
        self.dqn = build_dqn(learning_rate, state_dim, action_dim, 16, 16)
        self.replay_memory = ReplayBuffer(memory_size, state_dim)
        self.dqn.summary()

    def do_action(self, observation, explore):
        if explore and np.random.rand() < self.eps:
            return np.random.choice(self.action_dim)
        else:
            q_values = self.dqn.predict(observation.reshape(1, env.state_dim_size()))
            return np.argmax(q_values)

    def store_transition(self, from_state, to_state, action, reward, done):
        self.replay_memory.store_transition(
                from_state, to_state, action, reward, done)

    def learn(self, batch_size):
        if batch_size > self.replay_memory.memory_counter:
            return np.Inf, self.eps
        
        from_states, to_states, actions, rewards, terminals = \
                self.replay_memory.get_batch(batch_size)

        q_this = self.dqn.predict(from_states)
        q_next = self.dqn.predict(to_states)

        q_target = np.copy(q_this)

        q_target[:, actions] = rewards + \
                np.where(terminals == False, self.gamma * np.max(q_next, axis=1), 0)

        loss = self.dqn.train_on_batch(from_states, q_target)

        self.eps = self.eps - self.eps_decrement \
                if self.eps > self.eps_end else self.eps_end

        return loss, self.eps
