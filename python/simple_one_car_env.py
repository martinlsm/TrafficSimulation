import env_api
import numpy as np
import random

class AgentEnvironment:
    def __init__(self, traffic_env_id, car_start, car_end):
        self.env = env_api.Environment(traffic_env_id)
        self.start_index = car_start
        self.end_index = car_end
        self.car_id = -1    # no car is active yet

    def state_dim_size(self):
        return 15

    def action_dim_size(self):
        return 5

    # this environment reduces the amount of possible actions to 5
    def __translate_action(self, action):
        if action == 0:
            return env_api.car_action_do_nothing
        elif action == 1:
            return env_api.car_action_gas_medium
        elif action == 2:
            return env_api.car_action_brake_medium
        elif action == 3:
            return env_api.car_action_turn_left_hard
        elif action == 4:
            return env_api.car_action_turn_right_hard

    def reset(self, place_car_randomly):
        if self.car_id != -1:
            self.env.remove_car(self.car_id)

        if place_car_randomly:
            self.car_id = self.env.spawn_car_at_random_pos(self.end_index)
        else:
            self.car_id = self.env.spawn_car(self.start_index, self.end_index)
        observation = self.env.read_state_sensors(self.car_id)
        reward = self.env.get_reward_advanced(self.car_id)
        done = self.env.in_terminal_state(self.car_id)
        return observation, reward, done

    def step(self, action):
        action = self.__translate_action(action)
        self.env.do_action(self.car_id, action)
        self.env.update()

        observation = self.env.read_state_sensors(self.car_id)
        reward = self.env.get_reward_advanced(self.car_id)
        done = self.env.in_terminal_state(self.car_id)

        return observation, reward, done

    def get_car_position(self):
        return self.env.get_car_position(self.car_id)

    def get_car_destination(self):
        return self.env.get_car_destination(self.car_id)
