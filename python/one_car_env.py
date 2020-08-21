import env_api
import numpy as np
import random

TRAFFIC_ENV_ID = 1
env_api.load_traffic_environment(TRAFFIC_ENV_ID)

def car_count():
    return 1

def state_dim_size():
    return 16

def action_dim_size():
    return env_api.action_dim_size()

def reset():
    global car_id

    try:
        env_api.remove_car(car_id)
    except NameError:
        pass

    # car_id = env_api.spawn_car(*random.sample(range(8), 2))
    car_id = env_api.spawn_car(0, 7)
    observation = env_api.read_state_sensors(car_id)
    reward = env_api.get_reward_advanced(car_id)
    done = env_api.in_terminal_state(car_id)
    return observation, reward, done

def step(action):
    action = env_api.index_to_action(action)
    env_api.do_action(car_id, action)
    env_api.update()

    observation = env_api.read_state_sensors(car_id)
    reward = env_api.get_reward_advanced(car_id)
    done = env_api.in_terminal_state(car_id)

    return observation, reward, done

