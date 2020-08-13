import env_api as env
import numpy as np
import random

TRAFFIC_ENV_ID = 1
env.load_traffic_environment(TRAFFIC_ENV_ID)

def car_count():
    return 1

def state_dim_size():
    return env.state_dim_size()

def action_dim_size():
    return env.action_dim_size()

def reset():
    global car_id
    # car_id = env.spawn_car(*random.sample(range(8), 2))
    car_id = env.spawn_car(0, 6)
    observation = env.read_state(car_id)
    reward = env.get_reward_simple(car_id)
    done = env.in_terminal_state(car_id)
    return observation, reward, done

def step(action):
    action = env.index_to_action(action)
    env.do_action(car_id, action)
    env.update()

    observation = env.read_state(car_id)
    reward = env.get_reward_simple(car_id)
    done = env.in_terminal_state(car_id)

    return observation, reward, done

