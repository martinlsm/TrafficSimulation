import numpy as np
import gym
from agent import CarAgent
import tensorflow as tf
import matplotlib.pyplot as plt

def plot_scores(scores):
    plt.scatter(np.arange(len(scores)), scores, label='Scores')
    plt.legend()
    plt.show()

if __name__ == '__main__':
    env = gym.make('CartPole-v0')
    lr = 0.001
    n_games = 500
    agent = CarAgent(gamma=0.99, learning_rate=lr,
                     state_dim=env.observation_space.shape[0],
                     action_dim=env.action_space.n,
                     eps_start=1.0, eps_decrement=0.001, eps_end=0.01,
                     memory_size=1000000)

    scores = []
    eps_history = []

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

        eps_history.append(agent.eps)
        scores.append(score)
        avg_score = np.mean(scores[-100:])
        print(f'Episode: {i}')
        print(f'  Average Score: {score}')

    plot_scores(scores)
