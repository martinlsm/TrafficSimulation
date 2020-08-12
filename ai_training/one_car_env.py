import env_api as env
import numpy as np
import random

TRAFFIC_ENV_ID = 1
env.load_traffic_environment(TRAFFIC_ENV_ID)

def reset():
    global car_id
    car_id = env.spawn_car(*random.sample(range(8), 2))

def step(action):
    action = env.index_to_action(action)
    env.do_action(car_id, action)
    env.update()

    observation = env.read_state(car_id)
    reward = env.get_reward(car_id)
    done = env.in_terminal_state(car_id)

    return observation, reward, done

