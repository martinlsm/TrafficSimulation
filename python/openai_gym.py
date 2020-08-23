import numpy as np
import gym
from agent import CarAgent
import tensorflow as tf

if __name__ == '__main__':
    env = gym.make('LunarLander-v2')
    lr = 0.001
    n_games = 500
    agent = CarAgent(gamma=0.99, learning_rate=lr,
                     state_dim=env.observation_space.shape[0],
                     action_dim=env.action_space.n,
                     eps_start=1.0, eps_decrement=0.001, eps_end=0.01,
                     memory_size=1000000)

    for i in range(n_games):
        done = False
        score = 0
        observation = env.reset()
        while not done:
            action = agent.do_action(observation, True)
            observation_, reward, done, info = env.step(action)
            score += reward
            agent.store_transition(observation, observation_, action, reward, done)
            observation = observation_
            agent.learn(64)

        print(score)
