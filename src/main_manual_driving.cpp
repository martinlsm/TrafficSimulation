#include <iostream>

#include <SFML/Graphics.hpp>
#include <vector>

#include "rendering.h"
#include "traffic_env.h"
#include "texture_manager.h"

using namespace traffic;

int main() {
	texture_manager::load_textures();
	TrafficEnvironment* environment = load_environment(1);
	WorldRenderer world_renderer {environment->road_system->road_pieces};

	unsigned long car_id = environment->spawnCar(0, 4);
	CarMechanics* car = environment->getCarMechanics(car_id);
	CarSprite car_sprite{car};

	vector<float> sensor_angles;
	for (int i = 0; i < 8; i++) {
		float f = i * M_PI / 4;
		sensor_angles.push_back(f);
	}
	SensorsRenderer sensors_renderer {environment->road_system, car, sensor_angles};

	sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(WORLD_WIDTH, WORLD_HEIGHT), "Traffic Simulation");
	window.setFramerateLimit(FRAME_RATE);

    while (window.isOpen()) {
		// handle input
        sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed
					|| event.key.code == sf::Keyboard::Q) {
				window.close();
			}
			
			car_action action;
			if (event.key.code == sf::Keyboard::W) {
				action = GAS_MEDIUM;
			} else if (event.key.code == sf::Keyboard::S) {
				action = BRAKE_MEDIUM;
			} else if (event.key.code == sf::Keyboard::A) {
				action = TURN_LEFT_HARD;
			} else if (event.key.code == sf::Keyboard::D) {
				action = TURN_RIGHT_HARD;
			} else {
				action = DO_NOTHING;
			}
			environment->doAction(car_id, action);
		}

		// update state
		sf::Time dt = clock.restart();
		float dt_sec = dt.asSeconds();
		environment->update(dt_sec);

		environment->clearFinishedCars();
		car_state state = environment->getCarState(car_id);

		std::cout << (int)state << std::endl;

		if (state == NOT_FOUND) {
			car_id = environment->spawnCar(0, 7);
			car = environment->getCarMechanics(car_id);
			car_sprite = CarSprite{car};
		}

		// render screen
        window.clear({0x43,0x8a, 0x5e});
		world_renderer.draw(window);
		car_sprite.draw(window);
		sensors_renderer.draw(window);
        window.display();
    }

	delete environment->road_system;
	delete environment;
	texture_manager::delete_textures();

    return 0;
}
