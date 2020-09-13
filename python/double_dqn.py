import tensorflow as tf
import gym
from tensorflow import keras
import numpy as np

class DuelingDeepQNetwork(keras.Model):
    def __init__(self, num_actions, dense_1_dims, dense_2_dims):
        super(DuelingDeepQNetwork, self).__init__()
        self.dense1 = keras.layers.Dense(dense_1_dims, activation='relu')
        self.dense2 = keras.layers.Dense(dense_2_dims, activation='relu')
        self.V = keras.layers.Dense(1, activation=None)
        self.A = keras.layers.Dense(num_actions, activation=None)

    def call(self, state):
        x = self.dense1(state)
        x = self.dense2(x)
        V = self.V(x)
        A = self.A(x)

        Q = V + (A - tf.math.reduce_mean(A, axis=1, keepdims=True))
        return Q

    def advantage(self, state):
        x = self.dense1(state)
        x = self.dense2(x)
        A = self.A(x)
        return A


class ReplayBuffer():
    def __init__(self, max_size, input_shape):
        self.current_size = 0
        self.max_size = max_size
        self.state_memory = np.zeros(shape=(max_size, *input_shape),
                                     dtype=np.float32)
        self.new_state_memory = np.zeros(shape=(max_size, *input_shape),
                                         dtype=np.float32)
        self.action_memory = np.zeros(max_size, dtype=np.int32)
        self.reward_memory = np.zeros(max_size, dtype=np.int32)
        self.terminal_memory = np.zeros(max_size, dtype=np.bool)

    def store_transition(self, state, new_state, action, reward, terminal):
        i = self.current_size % self.max_size
        self.state_memory[i] = state
        self.new_state_memory[i] = new_state
        self.action_memory[i] = action
        self.reward_memory[i] = reward
        self.terminal_memory[i] = terminal
        self.current_size += 1

    def sample_buffer(self, batch_size):
        mem_end = min(self.current_size, self.max_size)
        batch = np.random.choice(mem_end, batch_size, replace=False)
        
        states = self.state_memory[batch]
        new_states = self.new_state_memory[batch]
        actions = self.action_memory[batch]
        rewards = self.reward_memory[batch]
        terminals = self.terminal_memory[batch]

        return states, new_states, actions, rewards, terminals

class Agent():
    def __init__(self, learning_rate, gamma, num_actions, eps, batch_size,
                 input_shape, eps_dec=1e-3, eps_end=0.01, mem_size=1000000,
                 dense_1_dims=128, dense_2_dims=128, replace=100):
        self.action_space = [i for i in range(num_actions)]
        self.gamma = gamma
        self.epsilon = eps
        self.epsilon_dec = eps_dec
        self.epsilon_end = eps_end
        self.replace = replace
        self.batch_size = batch_size

        self.learn_step_counter = 0
        self.memory = ReplayBuffer(mem_size, input_shape)
        self.q_eval = DuelingDeepQNetwork(num_actions, dense_1_dims, dense_2_dims)
        self.q_next = DuelingDeepQNetwork(num_actions, dense_1_dims, dense_2_dims)

        self.q_eval.compile(
                optimizer=keras.optimizers.Adam(learning_rate=learning_rate),
                loss='mean_squared_error')
        self.q_next.compile(
                optimizer=keras.optimizers.Adam(learning_rate=learning_rate),
                loss='mean_squared_error')

    def store_transition(self, state, new_state, action, reward, terminal):
        self.memory.store_transition(
                state, new_state, action, reward, terminal)

    def choose_action(self, state):
        if np.random.rand() < self.epsilon:
            action = np.random.choice(self.action_space)
        else:
            state = np.array([state])
            action_q_vals = self.q_eval(state)
            action = tf.math.argmax(action_q_vals, axis=1).numpy()[0]
        return action

    def learn(self):
        if self.memory.current_size < self.batch_size:
            return

        if self.learn_step_counter % self.replace == 0:
            self.q_next.set_weights(self.q_eval.get_weights())


        states, new_states, actions, rewards, terminals = \
                self.memory.sample_buffer(self.batch_size)

        q_pred = self.q_eval(states)
        q_next = self.q_next(new_states)
        q_target = q_pred.numpy()
        max_actions = tf.math.argmax(self.q_eval(new_states), axis=1)

        for idx, terminal in enumerate(terminals):
            q_target[idx, actions[idx]] = rewards[idx] + \
                    self.gamma * q_next[idx, max_actions[idx]] \
                    * (1 - int(terminals[idx]))
        self.q_eval.train_on_batch(states, q_target)

        self.epsilon = self.epsilon - self.epsilon_dec \
                if self.epsilon > self.epsilon_end \
                else self.epsilon_end

        self.learn_step_counter += 1

if __name__ == '__main__':
    env = gym.make('LunarLander-v2')
    agent = Agent(learning_rate=0.0005, gamma=0.99, num_actions=4, eps=1.0,
                  batch_size=64, input_shape=[8])
    n_games = 500
    scores = []
    eps_history = []

    for i in range(n_games):
        done = False
        score = 0
        observation = env.reset()
        while not done:
            action = agent.choose_action(observation)
            next_observation, reward, done, info = env.step(action)
            score += reward
            agent.store_transition(
                    observation, next_observation, action, reward, done)
            observation = next_observation
            agent.learn()
        eps_history.append(agent.epsilon)
        scores.append(score)

        avg_score = np.mean(scores[-100:])
        print(f'Episode {i} average score: {avg_score}')

