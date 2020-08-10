import python_agent as pa

import unittest

class TestStringMethods(unittest.TestCase):

    def test_load_env_1(self):
        pa.load_traffic_environment(1)
        self.assertEqual(pa.car_count(), 0)
        self.assertEqual(pa.destination_count(), 8)

    def test_spawn_car(self):
        pa.load_traffic_environment(1)
        car_id = pa.spawn_car(0, 7)
        self.assertEqual(car_id, 1)
        state = pa.read_state(car_id)
        self.assertEqual(len(state), 6)
        reward = pa.get_reward(car_id)
        self.assertEqual(reward, 0)

if __name__ == '__main__':
    unittest.main()
