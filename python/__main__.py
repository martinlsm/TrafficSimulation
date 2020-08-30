import argparse

import numpy as np
import matplotlib.pyplot as plt
import pygame
import env_api
import agent
import simple_one_car_env as env

def argument_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument('-m', '--keras_model', type=str, default=None)
    parser.add_argument('-w', '--world', type=int, default=1)
    parser.add_argument('-s', '--start_index', type=int, default=0)
    parser.add_argument('-e', '--end_index', type=int, default=1)
    parser.add_argument('-b', '--batch_size', type=int, default=64)
    parser.add_argument('-n', '--num_updates', type=int, default=250)
    parser.add_argument('-lr', '--learning_rate', type=float, default=7e-3)
    parser.add_argument('-p', '--plot_results', action='store_true', default=False)
    args = parser.parse_args()
    return args

car_texture = pygame.image.load('assets/car.png')

def get_car_transform(rotation, size):
    car_transformed = pygame.transform.scale(car_texture, size)
    car_transformed = pygame.transform.rotate(car_transformed, 270 - rotation)
    return car_transformed

def render_cars(screen):
    car_ids = env_api.get_car_ids()
    for car_id in car_ids:
        position = env_api.get_car_position(car_id)
        rotation = env_api.get_car_rotation_degrees(car_id)
        size = env_api.get_car_size(car_id)
        
        adjusted_position = (round(position[0] - size[0] / 2),
                             round(position[1] - size[1] / 2))
        car_transform = get_car_transform(rotation, [int(x) for x in size])
        screen.blit(car_transform, adjusted_position)

def train(car_agent, env, batch_sz=64, updates=250):
    actions = np.empty((batch_sz,), dtype=np.int32)
    rewards, dones, values = np.empty((3, batch_sz))
    observations = np.empty((batch_sz,) + (env.state_dim_size(),))

    scores = [0.0]
    next_obs, _, _ = env.reset()
    for update in range(updates):
        for step in range(batch_sz):
            observations[step] = next_obs.copy()
            actions[step], values[step] = car_agent.model.action_value(next_obs[None, :])
            next_obs, rewards[step], dones[step] = env.step(actions[step])

            scores[-1] += rewards[step]
            if dones[step]:
                scores.append(0.0)
                next_obs, _, _ = env.reset()
                print("Episode: %03d, Score: %03d" % (len(scores) - 1, scores[-2]))

        _, next_value = car_agent.model.action_value(next_obs[None, :])
        returns, advs = car_agent._returns_advantages(rewards, dones, values, next_value)
        acts_and_advs = np.concatenate([actions[:, None], advs[:, None]], axis=-1)
        losses = car_agent.model.train_on_batch(observations, [acts_and_advs, returns])
        print("[%d/%d] Losses: %s" % (update + 1, updates, losses))
        print(f'Car Position: {env.get_car_position()}')
    return scores
  
def validation_render_episode(car_agent, background):
    print('Validation Round')
    pygame.init()
    clock = pygame.time.Clock()
    pygame.display.set_caption('Agent Training Episode')

    screen = pygame.display.set_mode((1000,800))

    state_counter = 0
    score = 0
    observation, reward, done = env.reset()
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
        pygame.draw.circle(screen, (255, 255, 255, 64), env.get_car_destination(), env_api.get_goal_margin())
        render_cars(screen)
        pygame.display.update()

    print(f'  Car position: {env.get_car_position()}')
    print(f'  Reward: {score}')
    print(f'  States: {state_counter}')

    pygame.quit()

def save_scores_plot(scores):
    plt.scatter(np.arange(len(scores)), scores)
    plt.title('Score for car agent')
    plt.ylabel('score')
    plt.xlabel('episode')
    plt.savefig('scores.pdf')

# new main
if __name__ == '__main__':
    args = argument_parser()
    world_id = args.world
    start_index = args.start_index
    end_index = args.end_index

    env.init(world_id, start_index, end_index)
    background = pygame.image.load(f'env_images/env_{world_id}.jpg')

    model = agent.Model(num_actions=env.action_dim_size())
    car_agent = agent.CarAgent(model, args.learning_rate)

    rewards_history = train(car_agent, env, args.batch_size, args.num_updates)
    print("Finished training. Testing...")
    validation_render_episode(car_agent, background)

    if args.plot_results:
        plt.style.use('seaborn')
        plt.plot(np.arange(0, len(rewards_history), 10), rewards_history[::10])
        plt.xlabel('Episode')
        plt.ylabel('Total Reward')
        plt.show()
