import env_api as env
import numpy as np
import random

TRAFFIC_ENV_ID = 1
env.load_traffic_environment(TRAFFIC_ENV_ID)

def state_dim_size():
    return 17

def action_dim_size():
    return 5

# this environment reduces the amount of possible actions to 5
def __translate_action(action):
    if action == 0:
        return env.car_action_do_nothing
    elif action == 1:
        return env.car_action_gas_medium
    elif action == 2:
        return env.car_action_brake_medium
    elif action == 3:
        return env.car_action_turn_left_hard
    elif action == 4:
        return env.car_action_turn_right_hard

def reset():
    global car_id
    car_id = env.spawn_car(0, 6)
    observation = env.read_state_simple(car_id)
    reward = env.get_reward_simple(car_id)
    done = env.in_terminal_state(car_id)
    return observation, reward, done

def step(action):
    action = __translate_action(action)
    env.do_action(car_id, action)
    env.update()

    observation = env.read_state_simple(car_id)
    reward = env.get_reward_simple(car_id)
    done = env.in_terminal_state(car_id)

    return observation, reward, done

