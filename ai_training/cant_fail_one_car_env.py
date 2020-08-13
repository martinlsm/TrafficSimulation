import env_api as env
import numpy as np
import random

TRAFFIC_ENV_ID = 1
env.load_traffic_environment(TRAFFIC_ENV_ID)

def state_dim_size():
    return env.state_dim_size()

def action_dim_size():
    return 3

# in this environment, the car can't turn
def __translate_action(action):
    if action == 0:
        return env.car_action_do_nothing()
    elif action == 1:
        return env.car_action_gas_medium()
    elif action == 2:
        return env.car_action_brake_medium()

def reset():
    global car_id
    car_id = env.spawn_car(0, 6)
    observation = env.read_state(car_id)
    reward = env.get_reward_simple(car_id)
    done = env.in_terminal_state(car_id)
    return observation, reward, done

def step(action):
    action = __translate_action(action)
    env.do_action(car_id, action)
    env.update()

    observation = env.read_state(car_id)
    reward = env.get_reward_simple(car_id)
    done = env.in_terminal_state(car_id)

    return observation, reward, done

