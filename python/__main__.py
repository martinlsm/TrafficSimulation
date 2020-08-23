import numpy as np
import pygame
import env_api
import agent
import simple_one_car_env as env

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

def validation_render_episode(car_agent):
    print('Validation Round')
    pygame.init()
    clock = pygame.time.Clock()
    pygame.display.set_caption('Agent Training Episode')

    background = pygame.image.load('env_images/env_2.jpg')
    screen = pygame.display.set_mode((1000,800))

    state_counter = 0
    observation, reward, done = env.reset()
    while not done:
        action = car_agent.do_action(observation, False)
        next_observation, reward, done = env.step(action)
        if np.all(observation == next_observation):
            done = True

        observation = next_observation
        state_counter += 1

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                done = True
        dt = clock.tick(30)
        screen.blit(background, (0, 0))
        render_cars(screen)
        pygame.display.update()

    print(f'  Car position: {env.get_car_position()}')
    print(f'  Reward: {reward}')
    print(f'  States: {state_counter}')

    pygame.quit()

if __name__ == '__main__':
    car_agent = agent.CarAgent(1e-3, 0.99, env.state_dim_size(), env.action_dim_size(), 1.0, 0.00001, 0.035, 100000)

    training_rounds = 100000
    batch_size = 64
    state_counter = 0
    loss_history = np.zeros(1000000)

    rendering_on = True

    for i in range(1, training_rounds + 1):
        print(f'Training round #{i}')
        observation, reward, done = env.reset()
        assert reward == 0
        assert done == False
        while not done:
            action = car_agent.do_action(observation, True)
            next_observation, reward, done = env.step(action)
            car_agent.store_transition(observation, next_observation, action, reward, done)
            observation = next_observation
            loss, epsilon = car_agent.learn(batch_size)
            loss_history[state_counter] = loss
            state_counter += 1

        print(f'  Car position: {env.get_car_position()}')
        print(f'  Reward: {reward}')
        print(f'  States: {state_counter}')
        print(f'  Epsilon: {epsilon:.2f}')
        state_counter = 0

        car_agent.save_dqn_to_file('one_car_env.h5')

        if rendering_on and i % 5 == 0:
            validation_render_episode(car_agent)
