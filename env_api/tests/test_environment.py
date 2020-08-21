import env_api as env

import unittest

class TestStringMethods(unittest.TestCase):

    def test_load_env_1(self):
        env.load_traffic_environment(1)
        self.assertEqual(env.car_count(), 0)
        self.assertEqual(env.destination_count(), 8)

    def test_spawn_car(self):
        env.load_traffic_environment(1)
        car_id = env.spawn_car(0, 7)
        self.assertEqual(car_id, 1)
        state = env.read_state_simple(car_id)
        self.assertEqual(len(state), 8)
        reward = env.get_reward(car_id)
        self.assertEqual(reward, 0)

    def test_dim_size(self):
        env.load_traffic_environment(1)
        state_dim = env.state_dim_size()
        action_dim = env.action_dim_size()
        self.assertEqual(state_dim, 8)
        self.assertEqual(action_dim, 12)

if __name__ == '__main__':
    unittest.main()
