import python_agent
import numpy as np
import random

TRAFFIC_ENV_ID = 1
python_agent.load_traffic_environment(TRAFFIC_ENV_ID)

def reset():
    global car_id
    car_id = python_agent.spawn_car(*random.sample(range(8), 2))

def step(action):
    action = python_agent.index_to_action(action)
    python_agent.do_action(car_id, action)
    python_agent.update()

    observation = python_agent.read_state(car_id)
    reward = python_agent.get_reward(car_id)
    done = python_agent.in_terminal_state(car_id)

    return observation, reward, done

