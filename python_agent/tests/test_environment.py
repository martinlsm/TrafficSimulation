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

    def test_upper(self):
        self.assertEqual('foo'.upper(), 'FOO')

    def test_isupper(self):
        self.assertTrue('FOO'.isupper())
        self.assertFalse('Foo'.isupper())

    def test_split(self):
        s = 'hello world'
        self.assertEqual(s.split(), ['hello', 'world'])
        # check that s.split fails when the separator is not a string
        with self.assertRaises(TypeError):
            s.split(2)

if __name__ == '__main__':
    unittest.main()
