import argparse

import numpy as np
import matplotlib.pyplot as plt
import pygame
import agent
from simple_one_car_env import AgentEnvironment

def argument_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument('-m', '--keras_model', type=str, default=None)
    parser.add_argument('-w', '--world', type=int, default=1)
    parser.add_argument('-s', '--start_index', type=int, default=0)
    parser.add_argument('-e', '--end_index', type=int, default=1)
    parser.add_argument('-b', '--batch_size', type=int, default=64)
    parser.add_argument('-n', '--training_rounds', type=int, default=3000)
    parser.add_argument('-lr', '--learning_rate', type=float, default=7e-3)
    args = parser.parse_args()
    return args

car_texture = pygame.image.load('assets/car.png')

def get_car_transform(rotation, size):
    car_transformed = pygame.transform.scale(car_texture, size)
    car_transformed = pygame.transform.rotate(car_transformed, 270 - rotation)
    return car_transformed

def render_cars(env, screen):
    # FIXME: env.env is not good naming...
    car_ids = env.env.get_car_ids()
    for car_id in car_ids:
        position = env.env.get_car_position(car_id)
        rotation = env.env.get_car_rotation_degrees(car_id)
        size = env.env.get_car_size(car_id)

        adjusted_position = (round(position[0] - size[0] / 2),
                             round(position[1] - size[1] / 2))
        car_transform = get_car_transform(rotation, [int(x) for x in size])
        screen.blit(car_transform, adjusted_position)

def validation_render_episode(env, car_agent, background):
    print('Validation Round')
    pygame.init()
    clock = pygame.time.Clock()
    pygame.display.set_caption('Agent Training Episode')

    screen = pygame.display.set_mode((1000,800))

    state_counter = 0
    score = 0
    observation, reward, done = env.reset(False)
    while not done:
        action = car_agent.choose_action(observation)
        next_observation, reward, done = env.step(action)
        if np.all(observation == next_observation):
            done = True

        observation = next_observation
        state_counter += 1
        score += reward

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                done = True
        dt = clock.tick(30)
        screen.blit(background, (0, 0))
        pygame.draw.circle(screen, (255, 255, 255, 64),
                           env.get_car_destination(), env.env.get_goal_margin())
        render_cars(env, screen)
        pygame.display.update()

    print(f'  Car position: {env.get_car_position()}')
    print(f'  Reward: {score}')
    print(f'  States: {state_counter}')

    pygame.quit()

    return score

def save_scores_plot(scores):
    plt.scatter(np.arange(len(scores)), scores)
    plt.title('Score for car agent')
    plt.ylabel('score')
    plt.xlabel('episode')
    plt.savefig('scores.pdf')

if __name__ == '__main__':
    # Setup environment and agent
    args = argument_parser()
    world_id = args.world
    start_index = args.start_index
    end_index = args.end_index
    batch_size = args.batch_size
    training_rounds = args.training_rounds

    env = AgentEnvironment(world_id, start_index, end_index)
    background = pygame.image.load(f'env_images/env_{world_id}.jpg')

    model = agent.Model(num_actions=env.action_dim_size())
    car_agent = agent.CarAgent(model, args.learning_rate)

    # Start training
    actions = np.empty((batch_size,), dtype=np.int32)
    rewards, dones, values = np.empty((3, batch_size))
    observations = np.empty((batch_size,) + (env.state_dim_size(),))

    validation_scores = []
    next_obs, _, _ = env.reset(True)
    for update in range(training_rounds):
        for step in range(batch_size):
            observations[step] = next_obs.copy()
            actions[step], values[step] = car_agent.model.action_value(next_obs[None, :])
            next_obs, rewards[step], dones[step] = env.step(actions[step])

            if dones[step]:
                next_obs, _, _ = env.reset(True)

        _, next_value = car_agent.model.action_value(next_obs[None, :])
        returns, advs = car_agent._returns_advantages(rewards, dones, values, next_value)
        acts_and_advs = np.concatenate([actions[:, None], advs[:, None]], axis=-1)
        losses = car_agent.model.train_on_batch(observations, [acts_and_advs, returns])

        if update % 100 == 0:
            val_score = validation_render_episode(env, car_agent, background)
            validation_scores.append(val_score)

            plt.figure()
            plt.style.use('seaborn')
            plt.plot(validation_scores)
            plt.xlabel('Episode')
            plt.ylabel('Total Reward')
            plt.savefig('scores.pdf')
